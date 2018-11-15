from PIL import Image
import sys
import math

SIZE = 64
THRESHOLD = 100

# Data is a square image represented by a flat list
def convert_to_page_mode(data):
	BYTE_LENGTH = 8
	PAGE_WIDTH = 8
	n = math.sqrt(len(data))
	print("Length of data: ", n)
	if n != int(n):
		print("Image not square")
		exit()
	n = int(n)
	
	if n % 8 != 0:
		print("Image not a multiple of 8")
		exit()

	byte_list = []
	for page in range(int(n/PAGE_WIDTH)):
		for column in range(n):
			column_list = []
			for pixel in range(BYTE_LENGTH):
				column_list.append(
					data[page*n*PAGE_WIDTH + column + pixel*n])
			byte_list.append(list(reversed(column_list)))
	return byte_list

def mapping(pix):
	if (pix > THRESHOLD):
		return 1
	return 0

def convert_image(filename):
	im = Image.open(filename)
	im = im.resize((SIZE, SIZE))
	im = im.convert(mode='L', dither=None)
	im = im.point(mapping, mode='1')
	data = list(im.getdata())
	bytelist = convert_to_page_mode(data) # [["1", "0", "1" ...], ["1", "0"...]]
	bytelist = ["".join([str(bit) for bit in i]) for i in bytelist] # ["101...", "10..."]
	bytelist = [int(i, 2) for i in bytelist] #[64, 89, 18]
	print(len(bytelist), "len")
	print(bytelist)
	#bytelist = [chr(i) for i in bytelist] # ["a", "x", "?"]
	print(len(bytelist))
	f = open(filename[0:filename.find('.')] + str(SIZE) + str(".txt"), 'wb')
	f.write(bytearray(bytelist))
	f.close()

def main():
	convert_image(sys.argv[1])

if __name__ == "__main__":
	main()

