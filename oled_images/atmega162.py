import serial

class dummy_serial():


	output = ""

	def write(self, data):
		print(data.decode('utf-8'))

	def read(self):
		if (self.output == ""):
			self.output = input("$") + "\n"
		ch = self.output[0]
		self.output = self.output[1:]
		return bytes(ch, 'utf-8')

	def __init__(self):
		print("----- DUMMY SERIAL PORT -----")

def read_line(ser):
	line = ""
	ch = ''
	while(ch != '\n'):
		line +=ch
		ch = str(ser.read(), 'utf-8')
	return line.strip()

def write_line(line, ser):
	data = bytearray(line, "utf-8")
	ser.write(data)

def do_command(cmd, ser):
	if cmd.startswith("i"):
		with open(cmd[2:] + ".txt") as f:
			s = f.readline()
			if len(s) % 8 != 0:
				print("Error reading image")
				exit()
			write_line(s, ser)
			print("Sending image")


def main():
	port = "/dev/ttyS0"
	print("Now connected to " + port)
	ser = dummy_serial()
	#ser = serial.Serial(
	#	port=port,
	#	stopbits=serial.STOPBITS_TWO)
	while(True):
		line = read_line(ser)
		print("Atmega$ " + line)
		if line.startswith('@'):
			do_command(line[1:], ser)



if __name__ == "__main__":
	main()


