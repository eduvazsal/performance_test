#include<ros/ros.h>
#include<performance_test/SuperAwesome.h>
#include<performance_test/rateService.h>

double curRate;

bool rateServiceSend(performance_test::rateService::Request &req, performance_test::rateService::Response &resp) {
	resp.rate = curRate;
	return true;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "publish_message_cpp");
	ros::NodeHandle nh;
	ros::Publisher pub = nh.advertise<performance_test::SuperAwesome>("loop_rate", 1000);
	ros::ServiceServer rateServer = nh.advertiseService("rate", &rateServiceSend);

	double initRate = 0.1;
	double rateIncr = 0.1;
	curRate = initRate + rateIncr;

	ros::Rate rate(initRate);
	while(ros::ok()) {
		performance_test::SuperAwesome msg;
		msg.data = "Message";
		pub.publish(msg);
		ros::spinOnce();
		curRate += rateIncr;
		rate = ros::Rate(curRate);		
		rate.sleep();
	}
}