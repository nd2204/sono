if (Test-Path "C:\msys64") {
  $env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
  $env:Path = "C:\msys64\usr\bin;" + $env:Path
  $env:Path = "C:\msys64\mingw32\bin;" + $env:Path
  $env:Path = "C:\msys64\clang64\bin;" + $env:Path
  $env:Path = "C:\msys64\clangarm64\bin;" + $env:Path
  $env:Path = "C:\msys64\mingw32\bin;" + $env:Path
  $env:Path = "C:\msys64\mingw64\bin;" + $env:Path
} else {
  echo "MSYS2 not found, please install it from https://www.msys2.org/"
  exit 1
}

try {
  sh.exe build.sh $args
} catch {
  echo "sh.exe not found"
  echo "`r`nInstall msys2 from: https://www.msys2.org/"
}