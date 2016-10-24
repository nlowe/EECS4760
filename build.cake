#addin "Cake.Powershell"

var target = Argument("target", "Default");
var configuration = Argument("configuration", "Release");

Task("Clean")
    .Does(() =>
{
    CleanDirectories("./**/x64");
});

Task("Build")
    .Does(() =>
{
    MSBuild("./EECS4760.sln", cfg => cfg
        .SetVerbosity(Verbosity.Minimal)
        .SetConfiguration(configuration)
        .UseToolVersion(MSBuildToolVersion.VS2015)
        .SetMSBuildPlatform(MSBuildPlatform.x64)
    );
});

Task("Test")
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

    if(original != decrypted)
    {
        throw new Exception("Decrypted ciphertext is different from original plaintext");
    }
});

Task("Concat")
    .Does(() =>
{
    StartPowershellFile("./tools/Concat-Files.ps1");
});

Task("Default")
  .IsDependentOn("Build");

RunTarget(target);
