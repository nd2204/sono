if (Test-Path "C:\msys64\usr\bin") {
  echo "Found MSYS2"
  $env:Path = "C:\msys64\usr\bin;" + $env:Path
} else {
  echo "MSYS2 not found, please install it from https://www.msys2.org/"
  exit 1
}


try {
  sh.exe build.sh $args
} catch {
  echo "sh.exe doesn't found"
  echo "`r`nInstall msys2 from: https://www.msys2.org/"
}