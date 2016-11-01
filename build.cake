#addin "Cake.Powershell"

var target = Argument("target", "Default");
var configuration = Argument("configuration", "Release");

var weakKeySetting = Argument("DisallowWeakKeys", "Yes");
var semiWeakKeySetting = Argument("DisallowSemiWeakKeys", "Yes");
var possiblyWeakKeySetting = Argument("DisallowPossiblyWeakKeys", "Warn");

Task("Clean")
    .Does(() =>
{
    CleanDirectories("./**/x64");
});

Task("Build")
    .Does(() =>
{
    var weakKeyProp         = weakKeySetting == "Yes" ?
        "ENFORCE_NO_WEAK_KEYS" : weakKeySetting == "Warn" ?
            "WARN_WEAK_KEYS" : "NOENFORCE_WEAK_KEYS";
    var semiWeakKeyProp     = semiWeakKeySetting == "Yes" ?
        "ENFORCE_NO_SEMI_WEAK_KEYS" : semiWeakKeySetting == "Warn" ?
            "WARN_SEMI_WEAK_KEYS" : "NOENFORCE_SEMI_WEAK_KEYS";
    var possiblyWeakKeyProp = possiblyWeakKeySetting == "Yes" ?
        "ENFORCE_NO_POSSIBLY_WEAK_KEYS" : possiblyWeakKeySetting == "Warn" ?
            "WARN_POSSIBLY_WEAK_KEYS" : "NOENFORCE_POSSIBLY_WEAK_KEYS";

    Information("Weak Key Setting: " + weakKeyProp);
    Information("Semi-Weak Key Setting: " + semiWeakKeyProp);
    Information("Possibly-Weak Key Setting: " + possiblyWeakKeyProp);

    MSBuild("./EECS4760.sln", cfg => cfg
        //.SetVerbosity(Verbosity.Minimal)
        .SetConfiguration(configuration)
        .UseToolVersion(MSBuildToolVersion.VS2015)
        .SetMSBuildPlatform(MSBuildPlatform.x64)
        .WithProperty(weakKeyProp, "1")
        .WithProperty(semiWeakKeyProp, "1")
        .WithProperty(possiblyWeakKeyProp, "1")
    );
});

Task("Test-ECB")
    .IsDependentOn("Build")
    .Does(() =>
{
    var encryptedFile = "./" + (Guid.NewGuid()).ToString() + ".des";
    var decryptedFile = "./" + (Guid.NewGuid()).ToString() + ".desdec";

    var encryptExitCode = StartProcess("./x64/" + configuration + "/DES.exe", new ProcessSettings ()
        .WithArguments(args => args
            .Append("-e")
            .AppendQuoted("Pa$$w0rd")
            .Append("ECB")
            .AppendQuoted("./Test Files/Shakespeare.txt")
            .AppendQuoted(encryptedFile)
        )
    );

    if(encryptExitCode != 0) throw new Exception("Encryption failed with exit code " + encryptExitCode);

    var decryptExitCode = StartProcess("./x64/" + configuration + "/DES.exe", new ProcessSettings ()
        .WithArguments(args => args
            .Append("-d")
            .AppendQuoted("Pa$$w0rd")
            .Append("ECB")
            .AppendQuoted(encryptedFile)
            .AppendQuoted(decryptedFile)
        )
    );

    DeleteFile(encryptedFile);
    if(decryptExitCode != 0) throw new Exception("Decryption failed with exit code " + decryptExitCode);

    var original = CalculateFileHash("./Test Files/Shakespeare.txt").ToHex();
    var decrypted = CalculateFileHash(decryptedFile).ToHex();

    DeleteFile(decryptedFile);

    Information("Original Plaintext:  " + original);
    Information("Decrypted Plaintext: " + decrypted);

    if(original != decrypted)
    {
        throw new Exception("Decrypted ciphertext is different from original plaintext");
    }
});

Task("Test-CBC")
    .IsDependentOn("Build")
    .Does(() =>
{
    var encryptedFile = "./" + (Guid.NewGuid()).ToString() + ".des";
    var decryptedFile = "./" + (Guid.NewGuid()).ToString() + ".desdec";

    var encryptExitCode = StartProcess("./x64/" + configuration + "/DES.exe", new ProcessSettings ()
        .WithArguments(args => args
            .Append("-e")
            .AppendQuoted("Pa$$w0rd")
            .Append("CBC")
            .AppendQuoted("./Test Files/Shakespeare.txt")
            .AppendQuoted(encryptedFile)
        )
    );

    if(encryptExitCode != 0) throw new Exception("Encryption failed with exit code " + encryptExitCode);

    var decryptExitCode = StartProcess("./x64/" + configuration + "/DES.exe", new ProcessSettings ()
        .WithArguments(args => args
            .Append("-d")
            .AppendQuoted("Pa$$w0rd")
            .Append("CBC")
            .AppendQuoted(encryptedFile)
            .AppendQuoted(decryptedFile)
        )
    );

    DeleteFile(encryptedFile);
    if(decryptExitCode != 0) throw new Exception("Decryption failed with exit code " + decryptExitCode);

    var original = CalculateFileHash("./Test Files/Shakespeare.txt").ToHex();
    var decrypted = CalculateFileHash(decryptedFile).ToHex();

    DeleteFile(decryptedFile);

    Information("Original Plaintext:  " + original);
    Information("Decrypted Plaintext: " + decrypted);

    if(original != decrypted)
    {
        throw new Exception("Decrypted ciphertext is different from original plaintext");
    }
});

Task("Test")
    .IsDependentOn("Test-ECB")
    .IsDependentOn("Test-CBC");

Task("Concat")
    .Does(() =>
{
    StartPowershellFile("./tools/Concat-Files.ps1");
});

Task("Default")
  .IsDependentOn("Build");

RunTarget(target);
