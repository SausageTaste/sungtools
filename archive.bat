@echo off

for /F "tokens=2 delims==" %%G in ('wmic OS Get localdatetime /value') do set "datetime=%%G"

set "sortable_date=%datetime:~0,4%-%datetime:~4,2%-%datetime:~6,2%"
set "sortable_time=%datetime:~8,2%-%datetime:~10,2%-%datetime:~12,2%"
set "sortable_datetime=%sortable_date%_%sortable_time%"
set "folder_name=sungtoolscpp_archive_%sortable_datetime%"

mkdir %folder_name%
echo * Created a folder: ./%folder_name%

git archive --format=zip --output="%folder_name%/source.zip" HEAD
echo * Source archive created

git bundle create "%folder_name%/patch.bundle" --all --tags
echo * Git patch bundle created

pause
