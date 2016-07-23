#include <iostream>
#include <string>
#include <vector>

extern "C" {
	#include "unistd.h"
}

#include "DataStructures/DecodedPacket.h"
#include "Interface/PythonInterface.h"
#include "Interface/Plot.h"
#include "Interface/RS232.h"
#include "System/Util.h"
#include "DataStructures/Packet.h"
#include "System/Log.h"
#include "System/JSONLoader.h"

//int pollCom();

int main(void) {

	/*BPP::JSONLoader settings("Prefs/settings.json");
	std::cout << "Balloon Calls:\n";
	std::vector<std::string> bCalls = settings.getBalloonCalls();
	for(size_t i=0; i<bCalls.size(); i++) {
		std::cout << bCalls[i] << std::endl;
	}
	std::cout << "Van Calls:\n";
	std::vector<std::string> vCalls = settings.getVanCalls();
	for(size_t i=0; i<vCalls.size(); i++) {
		std::cout << vCalls[i] << std::endl;
	}
	std::cout << "Unparsed Log:\n";
	std::cout << settings.getUnparsedLogFile() << std::endl;
	std::cout << "Parsed Log:\n";
	std::cout << settings.getParsedLogFile() << std::endl;

	std::string instDirectory = settings.getInstallDirectory(); */
	std::string instDirectory = "/home/nick/Code/Link-TLM/";

	BPP::clearTerm();
	usleep(5000000);

	BPP::DecodedPacket packet("W3EAX-9", std::string("2015-06-13 07:17:44 EDT"), 39.657036, -77.934181, 606, 500, -1, std::string("W3EAX-9 www.umd.edu"));
	BPP::DecodedPacket pkt2;

	std::cout << "Callsign: " << packet.callsign << std::endl;
	std::cout << "Time: " << packet.timestamp << std::endl;
	std::cout << "Lat/Lon: " << packet.lat << " " << packet.lon << std::endl;
	std::cout << "Alt (ft): " << packet.alt << std::endl;
	std::cout << "Comment: " << packet.comment << std::endl;

	pkt2 = packet;
	std::cout << "CS2: " << pkt2.callsign << std::endl;
	BPP::PythonInterface::initPython(instDirectory);

	std::cout << BPP::PythonInterface::getString("BPPparser", "getLon", "W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX") << std::endl;
	std::cout << BPP::PythonInterface::getInt("BPPparser", "getAlt", "W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX") << std::endl;
	std::cout << BPP::PythonInterface::getFloat("BPPparser", "getSpd", "W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX") << std::endl;
	
	//BPP::TempParse parse("W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX");
	//BPP::TempParse parse2("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000606|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev");

	//parse.parse();
	//parse.print();

	BPP::Packet p1("W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX");
	BPP::Packet p2("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000606|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev");

	p2.init("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000606|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev");

	p1.parse();
	std::cout << "P1 Valid: " << p1.isValid() << std::endl << std::endl;
	p1.print();
	std::cout << std::endl;
	std::cout << p1.getCall() << std::endl << std::endl;
	pkt2 = p1.getPacket();
	std::cout << "P1 getPacket Test:\n" << pkt2.callsign << std::endl << pkt2.timestamp << std::endl << pkt2.lat << std::endl;

	BPP::clearTerm();
	usleep(5000000);

	p2.parse();
	std::cout << "P2 Valid: " << p2.isValid() << "\n\n";
	p2.print();
	std::cout << std::endl;
	std::cout << p2.getCall() << std::endl << std::endl;

	//parse2.parse();
	//parse2.print();
	//std::cout << parse2.getCall() << std::endl;

	BPP::clearTerm();
	usleep(5000000);

	BPP::Packet p3("WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000");
	p3.parse();
	std::cout << "P3 Valid: " << p3.isValid() << std::endl;

/*	BPP::clearTerm();
	usleep(5000000);

	BPP::Log test;
	test.open("test.txt");
	test.log();
	std::cout << "logging\n";
	test.log(1);
	std::cout << "logging\n";
	test.log("HI! ",1," ",3.1415);
	std::cout << "logging\n"; */

	BPP::clearTerm();
	usleep(5000000);

	BPP::Packet timepack1("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000600|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev");
	timepack1.parse();
	usleep(5000000);
	BPP::Packet timepack2("KB3ZZI-9>APRS,N3KTX-4*,WIDE1*,WIDE2-1,qAR,K3PDK-1:!/:Ig\\:iOfO   /A=000625|!!|  /KB3ZZI,7,8,27'C,http://goo.gl/dGbpev");
	timepack2.parse();
	//timepack2.calcAscentRate(timepack1);
	timepack2.print();

/*	unsigned char buf[4096];
	int n;
	std::string mode = "8N1";
	std::string oldData;
	std::string data;
	if(RS232_OpenComport(24, 9600, mode.c_str())) {
		std::cout << "NO OPEN PORT" << std::endl;
		return 0;
	}
	while(1) {
		n = RS232_PollComport(24, buf, 4095);
		oldData = data;

    	if(n > 0) {
      		buf[n] = 0;
		}
		data = (char*)buf;
		if(data != oldData) {
			std::cout << data << "END" << std::endl;
		}

		//pollCom();

		usleep(1000000);
	}

	BPP::RS232Serial serialPort;
	serialPort.portOpen("/dev/ttyUSB0", B9600, 8, 'N', 1);

	int n = 0;
	std::string data = "";
	std::string oldData = "";
	while(1) {
		oldData = data;
		n = serialPort.rxData();
		data = serialPort.getData();
		if(oldData != data) {
			std::cout << data << "END\n";
		}
		serialPort.portFlush();
		usleep(1000000);
	}

	std::cout << BPP::PythonInterface::getInt("BPPregex", "oldPacketMatch", "W3EAX-9>APT311,WIDE2-2:/203716h3859.58N/07656.35WO051/000/A=000111/W3EAX", instDirectory) << std::endl; */
/*	BPP::PythonInterface::initPython("/home/nick/Code/Link-TLM/");
	BPP::Plot testPlot;
	testPlot.init("Hi","x","y");
	testPlot.dividePlot(2);
	testPlot.labelSubplot(1,"ONE","","Y");
	testPlot.labelSubplot(2,"TWO","","YPrime");
	std::vector<float> testVal;
	testVal.push_back(2.0f);
	testVal.push_back(3.0f);
	testPlot.plotNewPoint(1.0f, testVal);
	testVal[0] = 3.0f;
	testVal[1] = 4.0f;
	testPlot.plotNewPoint(1.0f, testVal);

	char a;
	std::cin >> a; */

	BPP::PythonInterface::stopPython();

	return 0;
}

/*
int pollCom() {
	int n;
	unsigned char buf[4096];
	n = RS232_PollComport(24, buf, 4095);
	std::cout << "FUNC READ " << n << std::endl;
	return n;
} */
