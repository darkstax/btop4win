$bytes = [System.IO.File]::ReadAllBytes("C:\Users\StarL\Documents\AI_code\btop4win\x64\Release-LHM\btop4win.exe")
$text = [System.Text.Encoding]::Unicode.GetString($bytes)
$methods = @("GetCpuTemperature","GetProcessCount","GetVersion","GetGpuTemperature","GetGpuName","computeSelfHash","Authenticate","GetProcesses","IsAlive","invoke0","invoke1","invokeDouble")
foreach ($m in $methods) {
    $i = $text.IndexOf($m)
    if ($i -ge 0) { Write-Host ("$m -> FOUND at $i") }
    else { Write-Host ("$m -> NOT FOUND") }
}
