rm -rf Src/
rm -rf Inc/
CUBE=~/tools/STM32CubeMX/STM32CubeMX
java -jar $CUBE -q script -s
rm -rf MXTmpFiles

