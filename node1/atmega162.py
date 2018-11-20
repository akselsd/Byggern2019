import serial
import struct

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
	ch = str(ser.read().decode("utf-8", errors="ignore"))
	if ch == '':
		return None

	while(ch != '\n'):
		line +=ch
		ch = str(ser.read().decode("utf-8", errors="ignore"))
	return line.strip()

def write_line(line, ser):
	data = bytearray(line)
	#print(data)
	ser.write(data)

def getKeyStr(item):
	return item[4:]

def do_command(cmd, ser):
	if cmd.startswith("i"):
		# Image
		with open("oled_images/" + cmd[1:].strip() + ".txt", 'rb') as f:
			s = f.readlines()
			total = 0
			for i in s:
				total+=len(i)
			if total % 8 != 0:
				print("Error reading image")
				print(cmd[1:].strip() + ".txt")
				exit()
			for i in s:
				write_line(i, ser)
			print("Sending image")
	if cmd.startswith("lr"):
		# Read leaderboard
		with open("game_data/leaderboard.txt") as f:
			s = f.readlines()
			s = sorted(s, key=getKeyStr, reverse=True)
			n_lines = 4
			for n in range(n_lines):
				if n >= len(s):
					s.append("NaN 000\n")
				write_line(s[n].encode("utf-8"), ser)
			print("Sending leaderboard")

	if cmd.startswith("ls"):
		# Save to leaderboard
		score = cmd.split()[1]

		with open("game_data/leaderboard.txt", 'a') as f:
			name = input("Enter name:")
			while len(name) != 3:
				print("Name must be three characters!")
				name = input("Enter name:")

			line = name + " " + score.zfill(3) + "\n"
			f.write(line)
			print("Saved score")


def main():

	port_one = "/dev/ttyS0"
	print("Now connected to " + port_one)
	node_one = serial.Serial(
		port=port_one,
		stopbits=serial.STOPBITS_TWO)

	while(True):
		line = read_line(node_one)
		if line:
			print("Node 1$ " + line)
			if '@' in line:
				do_command(line.split('@')[1], node_one)


if __name__ == "__main__":
	main()
