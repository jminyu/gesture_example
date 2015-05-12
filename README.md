

#Function Reference
#Gesture Recognition using Dynamic Bayesian Networks

Jongmin Yu, jmyu@gist.ac.kr

이 문서는 Dynamic Bayesian Network (DBN) 기반의 Gesture recognition 을 위한 함수들을 포함하고 있습니다. 이 함수에는 손을 좌/우, 상/하, 앞/뒤로 움직이는 6가지 동작에 대한 인식을 위한 확률 모델이 포함되어 있습니다. 

Library 는 visual studio 2010 에서 제작되었으며, 개발 환경에 따라 Link 에 오류가 있을 수 있습니다. 
Library 에는 별도의 Tracking function 이 포함되어 있지 않습니다. Library 사용자는 gesture recognition 을 위해 인식 대상이 되는 오브젝트(손/머리) 등을 detection 혹은 tracking 하는 함수를 사용해야 합니다. 

예제를 포함한 Full Source code 는 다음 페이지에서 찾을 수 있으며, MS Kinect 를 사용한 Gesture Recognition module를 포함하고 있습니다. 
Github : https://github.com/jminyu/gesture_example.git

함수의 구성은 다음과 같습니다.
#int MDirection_feature(float current_coord[], float past_coord[]) 
#int Main_Axis(float distance[]) 
#float DBN(int ,int ,int) 
#int Recognition_DBN(int length, int motion_chain[])  

각 함수에 대한 자세한 설명은 다음과 같습니다. 

#int MDirection_feature(float current_coord[], float past_coord[])
Parameters	current_coord : float array-like, shape=[n_axis], coordinate of object at current frame
-	current_coord[0]: x coordinate
-	current_coord[1]: y coordinate
-	current_coord[2]: z coordinate
past_coord : float array-like, shape=[n_axis], coordinate of object at previous frame
-	past_coord[0]: x coordinate
-	past_coord[1]: y coordinate
-	past_coord[2]: z coordinate
Returns	direction : integer, Motion direction feature of target object, 6 direction (right, left, up,down, push, pull)
-	0 : No motion
-	1 : Right
-	2 : Left
-	3 : Up
-	4 : Down
-	5 : Pull
-	6 : Push
Description	연속된 frame 간의 object 의 좌표를 통해 Motion Direction Feature (MDF) 를 추출하는 함수.
MDF 는 추출된 motion vector 에서 가장 큰 값을 가진 성분을 말하며, 6가지 동작을 인식하기 위해 6가지 방향을 가지고 있다. 

#int Main_Axis(float distance[])
Parameters	distance : float array-like, shape=[], vector component of x,y,z axis of motion.
Returns	direction : integer, Motion direction feature of target object, 6 direction (right, left, up,down, push, pull)
-	0 : No motion
-	1 : Right
-	2 : Left
-	3 : Up
-	4 : Down
-	5 : Pull
-	6 : Push
Description	현재 프레임과 이전 프레임에서 추출된 데이터를 통해 계산된 motion vector 에서 가장 큰 성분 (x,y,z) 이 무엇인지 판별하는 함수

#int Recognition_DBN(int length, int motion_chain[])
Parameters	length : integer, length of motion chain (amount of frame of segmented gesture)
motion_chain : integer array like, shape=[n_mdf(motion direction feature)], maximum 90 (90frame – 3sec), set of MDF of consecutive frames.
Returns	Gesture : integer, code of gesture
 1: swipe right is recognized
 2: swipe left is recognized
 3: up gesture is recognized
 4: down gesture is recognized
 5: push gesture is recognized
 6: pull gesture is recognized
 0: error is occurs

Description	입력된 MDF 집합으로부터, 해당하는 Gesture 를 출력해주는 함수. 
사전에 구축된 6개의 DBN 와, 특정 동작으로부터 추출된 MDF 의 집합을 이용해 가장 큰 확률을 가지는 특정 DBN(특정 동작) 을 찾는 함수


#float DBN(int ,int ,int)
Parameters	Gesturecode : integer, code for DBN of specific gesture, option for selection of probability model for specific gesture.
Len : integer, length of motion chain (amount of frame of segmented gesture)
motion_chain : integer array like, shape=[n_mdf(motion direction feature)], maximum 90 (90frame – 3sec), set of MDF of consecutive frames.
Returns	Probability : float,
Probability of given observation(motion chain) which is computed by specific gesture.
Description	DBN 을 구축하고 확률을 계산하는 함수, 
Gesturecode 는 확률을 계산할 특정 DBN 를 구분하는 parameter 이다. 
Gesturecode 를 통해 확률을 구해야 할 DBN 이 특징되면, 해당 DBN 에 motion chain 을 Observation 으로 이용하여 확률을 구한다. 
