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
		return bytes(ch)

	def __init__(self):
		print("----- DUMMY SERIAL PORT -----")

def read_line(ser):
	line = ""
	ch = str(ser.read())
	if ch == '':
		return None

	while(ch != '\n'):
		line +=ch
		ch = str(ser.read())
	return line.strip()

def write_line(line, ser):
	data = bytearray(line)
	ser.write(data)

def do_command(cmd, ser):
	if cmd.startswith("i"):
		with open(cmd[1:].strip() + ".txt") as f:
			s = f.readline()
			if len(s) % 8 != 0:
				print("Error reading image")
				exit()
			write_line(s, ser)
			print("Sending image")


def main():
	port_one = "/dev/ttyS0"
	print("Now connected to " + port_one)
	node_one = serial.Serial(
		port=port_one,
		timeout=0,
		stopbits=serial.STOPBITS_TWO)

	port_two = "/dev/ttyACM0"
	print("Now connected to " + port_two)
	node_two = serial.Serial(
		port=port_two,
		timeout=0,
		stopbits=serial.STOPBITS_TWO)

	while(True):
		line = read_line(node_one)
		if line:
			print("Node 1$ " + line)
			if line.startswith('@'):
				do_command(line[1:], node_one)
		#line = read_line(node_two)
		#if line:
		#	print("Node 2$ " + line)




if __name__ == "__main__":
	main()


