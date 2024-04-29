#!/bin/bash

for file in *.elf; do
	filename=${file%.*}
	/home/rriccio/riscv/bin/riscv32-unknown-elf-objcopy -O binary $file $filename.bin
	cp $filename.bin ../Demo/$filename.bin
	mv $filename.bin ../Logisim/$filename.bin
	#rm $file
	#rm $file.bin
	echo "Generated bin: ${filename}.bin"
done
