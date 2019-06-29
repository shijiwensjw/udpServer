#include <ros/ros.h>
#include <tf/tf.h>
#include <tf/transform_datatypes.h>
#include <actionlib/client/simple_action_client.h>
#include <ctime>

#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <nav_msgs/Odometry.h>

#include <tf/transform_listener.h>

#include "input.h"
#
using namespace std;


boost::shared_ptr<InputSocket> input_;

boost::thread* recvudp_thread_;
int packet_size_ = 0;

int pose_id;

/**
 * @brief "cmd_vel"topic的回调函数，并通过udp发送数据
 * @param msg
 */
void action_callback(std_msgs::Int32ConstPtr msg)
{
	pose_id = msg->data;
 	//cout<< pose_id << endl;
        ROS_INFO("pose_id:%d\n", pose_id);

	char buff[512] = {0};  
	uint8_t msg_len= 0;
    // 帧头
	buff[msg_len++] = 27;
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;
	buff[msg_len++] = 0;

	// 以下开始为消息
	// msg_len += sprintf(buff+msg_len, "%d", 20);//0

	buff[4] = pose_id;  // 消息的长度填充到第二个字节
	input_->sendPacket((uint8_t*)buff, msg_len);
	cout << buff+12 << endl;
}



int main(int argc, char** argv)
{
	ROS_INFO("===========000====================");
	ros::init(argc, argv, "udpServer");
	ros::NodeHandle n;
	ros::NodeHandle private_nh("~");
	ROS_INFO("===============================");
	//ros::Publisher  initPose_pub = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10);
	ros::Subscriber action_sub = n.subscribe("/action_recognize", 10, action_callback);

	

	private_nh.param<int>("packet_size", packet_size_, 10);


	// read data from live socket
	input_.reset(new InputSocket(private_nh));
    


	ros::Rate rate(1000);
	ros::spin();

	return 0;
}
