#include<ros/ros.h>
#include<performance_test/SuperAwesome.h>
#include<performance_test/rateService.h>

ros::ServiceClient rateClient;
bool firstFlag = true;
bool maxFlag = false;

void rateMessageReceived(const performance_test::SuperAwesome& msg) {
	performance_test::rateService::Request req;
	performance_test::rateService::Response resp;
	double curTime;
	static double prevTime;
	double realRate;
	double thres;

	bool success = rateClient.call(req, resp);

	if (!success) {
		ROS_ERROR_STREAM("Failed to call rate service");
	} else if (firstFlag) {
		prevTime = ros::Time::now().toSec();
		firstFlag = false;
	} else {
		curTime = ros::Time::now().toSec();
		realRate = 1.0/(curTime - prevTime);
		prevTime = curTime;
		thres = 0.5*resp.rate;

		if ((abs(resp.rate - realRate) > thres) && !maxFlag) {
			ROS_INFO_STREAM("Max rate: " << resp.rate << " Hz");
			maxFlag = true;
		}
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "subscribe_to_message_cpp");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("loop_rate", 1000, &rateMessageReceived);
	rateClient = nh.serviceClient<performance_test::rateService>("rate");

	ros::spin();
}