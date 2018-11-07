#!/usr/bin/env python
import rospy
from performance_test.msg import SuperAwesome
from performance_test.srv import rateService

curRate	= 0.1

def rateServiceSend(req):
	global curRate
	return curRate

def pyPublisher():
	rospy.init_node('publish_message_py')
	pub = rospy.Publisher("loop_rate", SuperAwesome, queue_size = 1000)
	rateServer = rospy.Service('rate', rateService, rateServiceSend)

	initRate = 0.1
	rateIncr = 0.1
	global curRate
	curRate = initRate + rateIncr

	rate = rospy.Rate(initRate)
	while not rospy.is_shutdown():
		msg = SuperAwesome()
		msg.data = "Message"
		pub.publish(msg)
		curRate += rateIncr
		rate = rospy.Rate(curRate)		
		rate.sleep()


if __name__ == '__main__':
	try:
		pyPublisher()
	except rospy.ROSInterruptException:
		pass

