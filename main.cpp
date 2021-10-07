#include <iostream>
#include <boost/asio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
using namespace boost::asio;
using namespace cv;
using ip::tcp;
using std::cout;
using std::endl;
using std::string;

int main()
{
    boost::asio::io_service io_service;
    // Step 1. Assume that the client application has already
    // obtained the IP-address and the protocol port number.
    std::string raw_ip_address = "192.168.40.107";
    unsigned short port_num = 1234;
    // Used to store information about error that happens
    // while parsing the raw IP-address.
    boost::system::error_code ec;
    // Step 2. Using IP protocol version independent address
    // representation.
    ip::address ip_address =
        ip::address::from_string(raw_ip_address, ec);
    // Step 3.
    ip::tcp::endpoint ep(ip_address, port_num);
    //socket creation
    tcp::socket socket(io_service);
    //connection
    socket.connect(ep);
    // request/message from client
    boost::asio::streambuf buff;
    std::ostream os(&buff);
    os << "Hello from Client!\n";
    boost::system::error_code error;
    boost::asio::write(socket, buff, error);
    if (!error)
    {
        cout << "Client sent hello message!" << endl;
    }
    else
    {
        cout << "send failed: " << error.message() << endl;
    }
    // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof)
    {
        cout << "receive failed: " << error.message() << endl;
    }
    else
    {
        const char *data = boost::asio::buffer_cast<const char *>(receive_buffer.data());
        cout << data << endl;
    }
    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;        // 0 = open default camera
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened())
    {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;
    int frameNumber = 0;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty())
        {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // send live and wait for a key with timeout long enough to show images
        boost::system::error_code error;
        os << frame;
        boost::asio::write(socket, buff, error);
        if (!error)
        {
            std::cout << "Send frame number " << frameNumber << std::endl;
        }
        else
        {
            cout << "send failed: " << error.message() << endl;
        }
        // imshow("Live", frame);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}