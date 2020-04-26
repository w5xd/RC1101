del RC1101-IMAGE.zip
@rem LCD can only open DOS 8.3 file names on the image
copy TS590andK3.4xe TS590.4xe
copy TS590andK3.4xe K3K2.4xe
copy TS590andK3.4xe RunFlash.4xe
copy IcomAndFlex.4xe Icom.4xe
copy IcomAndFlex.4xe Flex6000.4xe
7z a RC1101-IMAGE.ZIP *.d0* *.g0* *.dat *.gci *.txf *.4xe version.txt


