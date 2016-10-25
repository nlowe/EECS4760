$DESProjectPath = "$PSScriptRoot\..\DES"

$output = ""

Push-Location $DESProjectPath

Get-ChildItem -Recurse -Path $DESProjectPath -Include '*.c','*.cc','*.cxx','*.cpp','*.h','*.hxx','*.hpp' | ForEach-Object {
    $relativePath = ($_ | Resolve-Path -Relative)

    $output += "// |=================$("=" * ($relativePath.Length + 2))=================|`n"
    $output += "// |                  $relativePath                  |`n"
    $output += "// |=================$("=" * ($relativePath.Length + 2))=================|`n`n"

    $output += $_ | Get-Content -Raw
    $output += "`n"
}

Pop-Location

$output | Out-File -FilePath "$PSScriptRoot\..\concat.cxx" -Encoding ASCII
