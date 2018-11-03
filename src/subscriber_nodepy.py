#!/usr/bin/env python
import rospy
from performance_test.msg import SuperAwesome
from performance_test.srv import rateService

firstFlag = True
maxFlag = False
_prevTime = 0.0
rateClient = rospy.ServiceProxy('rate', rateService)

def rateCallback(msg):
	global rateClient
	global firstFlag
	global maxFlag
	global thres
	global _prevTime
	resp = rateClient()
	if firstFlag:
		_prevTime = rospy.get_time()
		firstFlag = False
	else: 
		curTime = rospy.get_time()
		realRate = 1.0/(curTime - _prevTime);
		_prevTime = curTime;
		thres = 0.5*resp.rate

		if ((abs(resp.rate - realRate) > thres) and not maxFlag):
			rospy.loginfo("Max rate: %d Hz" %resp.rate)
			maxFlag = True

def pySubscriber():
	rospy.init_node('subscribe_to_message_py')
	sub = rospy.Subscriber("loop_rate", SuperAwesome, rateCallback)	

	rospy.spin()

if __name__ == '__main__':
	try:
		pySubscriber()
	except rospy.ROSInterruptException:
		pass