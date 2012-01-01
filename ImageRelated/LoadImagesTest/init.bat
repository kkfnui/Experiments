for /L %%i in (2,1,10) do (
copy res\LargeImages\1.bmp res\LargeImages\%%i.bmp
)
for /L %%i in (2,1,1000) do (
copy res\SmallImages\1.bmp res\SmallImages\%%i.bmp
)
pause