#!/usr/bin/python

# This is a starmode application for metricom ricochet modems.
# It will place the specified modem into starmode, list the peers
# that it can see, and then ask them to identify themselves.
# It will also listen for incoming packets and respond in kind.

import getopt, io, sys, serial, signal, time

class Ricochet:
	#def __init__(self):
		
	def main(self, argv):
		port = '/dev/ttyACM0' # Set a default value for the modem port.
		try:
			opts, args = getopt.getopt(argv, "hm:", ['modem='])
		except getopt.GetoptError:
			print "Oh snap, a thing is wrong!\n"
			sys.exit(2)
		for opt, arg in opts:
			if opt == '-h':
				print "helptext"
			elif opt in ("-m", "--modem"):
				port = arg
				print "You chose a modem " + str(arg)
	
		self.ser = serial.Serial(port)
		self.modem = io.TextIOWrapper(io.BufferedRWPair(self.ser, self.ser, 1), newline = '\r', line_buffering = True)
		self.initModem()
		#self.atCommand('at~la')
		while 1:
			packet = self.getPacket()
			self.parsePacket(packet)
			
	def getPacket(self):
		packet = self.modem.readline().lstrip('*').rstrip('\r').split('*')
		# Let's chomp until we get a real packet.
		if len(packet) < 2:
			return self.getPacket()
		else:
			return packet
	
	def parsePacket(self, packet):
		tokens = packet
		if len(tokens) < 2:
			print 'Bad packet.'
			return
		if len(tokens) > 2:
			# We have extra tokens! Smoosh all the end ones together.
			print ''
		print str(tokens)
		#print "this packet has " + str(tokens) + " tokens\n"
			
	def initModem(self):
		#initString = "ate0q1dt**starmode\r**"
		self.modem.write(u"ate0dt**starmode\r")
		print"Initialized modem.\n"
		response = self.getPacket()
		#print str(response)
		self.getModemName()
		self.findNeighbors()
		
	def respondToIdentificationRequest(self):
		return ''
	
	def requestIdentification(self, neighbor):
		return ''
	
	def getModemName(self):
		self.atCommand('ATS306?')
		packet = self.getPacket()
		if packet[0] != '!MODEM':
			print 'ERROR: INCORRECT PACKET CAUGHT WHILE GETTING MODEM NAME'
			print str(packet)
			return
		data = packet[1].strip('\n').split('\n')
		data = data[1:-1]
		self.modemName = data[0]
		print 'Modem name is {0}'.format(self.modemName)
		return self.modemName

	def findNeighbors(self):
		self.atCommand('at~la')
		packet = self.getPacket()
		# print str(packet)
		# Perhaps a sanity check?
		if packet[0] != '!MODEM':
			print 'ERROR: INCORRECT PACKET CAUGHT WHILE DISCOVERING NEIGHBORS.'
			print str(packet)
			return
		# and the real response
		data = packet[1].strip('\n').split('\n')
		# the first and last token will be garbage
		data = data[1:-1]
		self.neighbors = data
		print 'Neighbors found:'
		for neighbor in data:
			print '   {0}'.format(neighbor)
		print 'done finding neighbors'
		
	def closePort(self):
		time.sleep(1)
		# drop out of starmode by sending the AT escape sequence
		self.modem.write(u'+++')
		time.sleep(1)
		self.ser.close()
	
	def atCommand(self, command):
		self.sendPacket("&COMMAND", command)

	def sendPacket(self, destination, data):
		self.modem.write(u"*{0}*{1}\r".format(destination, data))
			
### THE SCRIPT
ricochet = False 
def signalHandler(signal, frame):
	print "\nSIGINT CAUGHT. EXITING."
	ricochet.closePort()
	sys.exit(0)

if __name__ == "__main__":
	signal.signal(signal.SIGINT, signalHandler)
	ricochet = Ricochet()
	ricochet.main(sys.argv[1:])