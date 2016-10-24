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

Task("Default")
  .IsDependentOn("Build");

RunTarget(target);
