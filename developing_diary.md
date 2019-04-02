## 代码平台：RM开发板

## FreeRTOS: V9.00

## HAL库版本：1.13.0

## 作者：Satori

# 2019/2/13
建立了App和Bsp两个文件夹，分别放RTOS的task和bsp层的东西，目前打算先把遥控器数据的解码做掉，然后把can的发送做掉，最后实现一下遥控器控电机

主要工作看RemoteMsgTask.c和bsp_uart.c两个文件

# 2019/2/14

发现一个坑的要死的地方......

CUBEMX生成的功能引脚和板子上实际用的引脚不同的有好几个，就非常搞

遥控接收完成，但是是裸机的，实际上本来DJI的代码也没有用这个，但是有一个比较有意思的，专门有个任务用来执行模式切换，我觉得我们也可以尝试一下这个

# 2019/2/15

用信号量弄了一下模式切换
```
void RemoteMsg_Receive_ModeSw_TaskStart(void * argument)
{
    osEvent Event;
    while(1)
    {
        Event = osSignalWait(RC_MODE_SIGNAL | KM_MODE_SIGNAL | ST_MODE_SIGNAL , osWaitForever);

        //然后就是分一下模式咯
        if(Event.status == osEventSignal)
        {
            if(Event.value.signals & RC_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,SET);
            }
            if(Event.value.signals & KM_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,SET);
            }
            if(Event.value.signals & ST_MODE_SIGNAL)
            {
                HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,RESET);
                HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,RESET);
            }
        }
    }
}
```
就是这样的一个弱智玩意，感觉还是先设计一下整体的程序再去弄比较好，不然到时候写个乱七八糟的工程出来就太蠢了

大概理了一下程序，后面在这个基础上去再加更改吧
目前控制模式这样分：
```
typedef enum
{
chassis_follow_gimbal_remote_control，
chassis_follow_gimbal_keymouse_control，
chassis_free_keymouse_control，
chassis_sway，
chassis_stop，
}chassis_mode;

typedef enum
{
gimbal_remote_control，
gimbal_keymouse_control，
gimbal_auto，
gimbal_stop，
}gimbal_mode; 

typedef enum
{
shoot_auto，
shoot_remote_control，
shoot_keymouse_control，
shoot_stop，
}shoot_mode；
```
其中，如果是遥控器控制模式，则是
```
chassis_mode = CHASSIS_FOLLOW_GIMBAL_REMOTE_CONTROL

gimbal_mode = GIMBAL_REMOTE_CONTROL

shoot_mode = SHOOT_REMOTE_CONTROL/SHOOT_STOP
```

如果是停止模式，则是
```
chassis_mode = CHASSIS_STOP

gimabl_mode = GIMBAL_STOP

shoot_mode = SHOOT_STOP
```
如果是键盘控制模式，则是
```
chassis_mode = CHASSIS_FOLLOW_GIMBAL_KEYMOUSE_CONTROL/CHASSIS_FREE_KEYMOUSE_CONTROL/CHASSIS_SWAY/CHASSIS_STOP

gimbal_mode = GIMBAL_KEYMOUSE_CONTROL/GIMBAL_AUTO/GIMBAL_STOP

shoot_mode = SHOOT_KEYMOUSE_CONTROL/SHOOT_STOP
```
# 2019/2/16
代码的整体框架基本上搭建完毕，程序整体流程图也画了一部分，下面就是各个任务的具体实现上了，明天应该能够把底盘的task给完成掉
回头估计要建一个bsp_can来做can通信的相关参数
```
static void chassis_twist_handler(void)
{
  twist_count++;
  
  if (twist_side > 0)
  {
    if (gim.sensor.yaw_relative_angle >= 2*twist_angle)
    {
      twist_count = 0;
      twist_sign  = -1;
    }
    
    if(gim.sensor.yaw_relative_angle <= 0)
    {
      twist_count = 0;
      twist_sign  = 1;
    }
    
  }
  else
  {
    if (gim.sensor.yaw_relative_angle >= 0)
    {
      twist_count = 0;
      twist_sign  = -1;
    }
    
    if(gim.sensor.yaw_relative_angle <= -2*twist_angle)
    {
      twist_count = 0;
      twist_sign  = 1;
    }
    
  }
  chassis.position_ref = -twist_sign*twist_angle*cos(2*PI/twist_period*twist_count) + twist_side*twist_angle;
  
  chassis.vw = -pid_calc(&pid_chassis_angle, gim.sensor.yaw_relative_angle, chassis.position_ref);
  
}
```
ICRA的车的chassis_twist_handler

# 2019/2/17

把软件定时器开了，里面软件定时中断回调云台的控制任务和底盘的控制任务

然后刚才跑了一下程序发现及其快乐的事情，软件定时器没开启成功，最后出来的结果是osError，非常的愤怒不知道到底是个什么鬼

然后看了一下是因为在FreeRTOSConfig.h下面的配置就不对，这就是一个非常有趣的坑了，我敲了这么久代码甚至都没有想到先去把FreeRTOSConfig.h先给修改一下，绝了

然后就能成功跑软件定时器了，记住软件定时器的默认优先级是belowNormalPriority,所以其他任务如果不是以阻塞态运行就都能随随便便的打断它

还有一个非常坑的东西，就是我突然发现一开始我创建工程的时候貌似是打算只写个demo还是啥的，所以一开始初始化的串口中断只有串口6一个，这就非常的坑了，因为实际用的工程怕是好几个串口中断都需要用上，此时一名不知死活的肥宅进入了自闭状态，更惨的是我定时器的外设就初始化了一个6号定时器，真jb精彩

现在一个隐藏的雷区就是ICRA的车子弄了一堆定时器，而我并不知道它开这么多定时器究竟想做一些什么，搞得我非常非常慌

# 2019/2/18

开学没几天了嘤嘤嘤，先把can的收发写掉再说，由于目前没有mpu等乱七八糟玩意的资料，所以我现在也不太想继续细化我的程序框架图了

基本上写完了bsp_can，上手测试了一下自己写的can，结果

你写尼玛呢？？？？？？

非常非常的崩，跑了几次之后就会报can_error，我也很懵逼很不爽，艹

准确的说是这样的一个状态

整个CAN发送会有三次发送处于成功的，确实进入了HAL_CAN_STATE_BUSY_TX里面去，但是三次之后就会完美进入error，这就让我很懵逼

另外貌似can的接收中断也没有进去

主要涉及到的问题是`hcan1.Instance.TSR`这一位，这是正常发送的情况：

0x1C000003

而我的情况：

0x1C000000

0x19000008

0x52000008

0x82000008

emmmm好的吧，那我就看看到底是哪边会使得这个值变化，最惨的是，我没找到

好吧我tm就是个傻逼，我按照ICRA的外设配置进行的初始化，但是一点鸟用都没有，然后我就按照A型板的进行了一下配置，然后稳了
```
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_1TQ;
  hcan1.Init.BS1 = CAN_BS1_9TQ;
  hcan1.Init.BS2 = CAN_BS2_4TQ;
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = DISABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = DISABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = ENABLE;
```
贴一下'正确'的配置和'错误'的配置
```
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 5;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SJW = CAN_SJW_1TQ;
  hcan1.Init.BS1 = CAN_BS1_3TQ;
  hcan1.Init.BS2 = CAN_BS2_5TQ;
  hcan1.Init.TTCM = DISABLE;
  hcan1.Init.ABOM = ENABLE;
  hcan1.Init.AWUM = DISABLE;
  hcan1.Init.NART = DISABLE;
  hcan1.Init.RFLM = DISABLE;
  hcan1.Init.TXFP = DISABLE;
```
既然遥控器的数据也读出来了，电机也能动了，那么接下来就只剩下一件事情了，那就是......今晚写一个能动的底盘出来，嘿嘿嘿

# 2019/2/19

然后我就咕了，没来的及写完（哦豁，完蛋.jpg）

今天上午把到底盘电机的解算给写掉了，还是靠自己写工程才真实的感受到FreeRTOS的强大之处

第一，可以节省硬件资源，比如软件定时器功能，产生的软件定时中断效果和硬件定时中断没有感受到区别

第二，基于任务与任务通知的更高的抽象思路，可以使编程的思路变得更加清晰，所有的任务都可以抽象成线程，只要处理线程之间的关系就可以，而在这之前基于轮询-中断的代码要写起来就比较容易思路混乱，因为都是在各种中断回调里面乱丢东西

第三，更加灵活。尤其是osSignalSet()和osSignalWait()这两个函数，可以说用起来非常的爽，可以当成一种万能的'外部中断'来用，某种程度上甚至可以说OS其实拓展了32的中断系统

总的来说，我目前对于os的使用其实也才刚刚是冰山一角，但也切实的感受到os的功能真的非常非常强大，用好了可以非常好的帮助完成庞大的工程

喜出新bug，接收的时候进不了中断回调函数，然后就闭不了环，非常的郁闷了可以说
而且最谜的是我的发送是完全正常的，按照CAN的通讯协议，每一帧都应该是有Ack的，这就非常非常非常奇怪

总之发送是完全没有任何问题的，但是接收就从始至终都是一切为0，这使我极度郁闷

搞定了，初始化的过滤器弄得有点问题，我虽然感觉自己弄得和能动的这版毫无区别，但是最后就是读不出来东西，绝了

提一下这版能用的
```
void Can_Device_Init(CAN_HandleTypeDef* hcan)
{
  CAN_FilterConfTypeDef canfilter;
  
  static CanTxMsgTypeDef  Tx1Message;
  static CanRxMsgTypeDef  Rx1Message;
  static CanTxMsgTypeDef  Tx2Message;
  static CanRxMsgTypeDef  Rx2Message;
  
  canfilter.FilterMode              = CAN_FILTERMODE_IDMASK;
  canfilter.FilterScale             = CAN_FILTERSCALE_32BIT;
  
  canfilter.FilterIdHigh            = 0x0000;
  canfilter.FilterIdLow             = 0x0000;
  canfilter.FilterMaskIdHigh        = 0x0000;
  canfilter.FilterMaskIdLow         = 0x0000;
  
  canfilter.FilterFIFOAssignment    = CAN_FilterFIFO0;
  canfilter.FilterActivation        = ENABLE;
  canfilter.BankNumber              = 14;
  
  if(hcan == &hcan1)
  {
    canfilter.FilterNumber = 0;
    hcan->pTxMsg = &Tx1Message;
    hcan->pRxMsg = &Rx1Message;
  }
  if(hcan == &hcan2)
  {
    canfilter.FilterNumber = 14;
    hcan->pTxMsg = &Tx2Message;
    hcan->pRxMsg = &Rx2Message;
  }
  
  HAL_CAN_ConfigFilter(hcan, &canfilter);
  
}
```
# 2019/2/20
昨天想直接初始化的时候赋值一波pid参数，后来发现不靠谱，因为有这样的一个机制：
```
#include <stdio.h>

typedef struct
{
    int a;
    int b;
} B;

typedef struct
{
    int c;
    B d;
} A;

int main()
{
    A C.d={1,2};
    printf("%d",C.c);
}

这样的结构体赋值是无法成功的

#include <stdio.h>

typedef struct
{
    int a;
    int b;
} B;

typedef struct
{
    int c;
    B d;
} A;

int main()
{
    A C = {1,{2,3}};
    printf("%d",C.d.a);
}
```
而这种嵌套就可以了

调了一波底盘，动的和真的一样，不过使用起来依然只能说是游戏体验不是很佳，毕竟pid参数调的比较烂，回头可能会再调调，说到头那么大的静差是从哪里来的就搞得我很懵逼

还有一个很讨厌的事情，RM开发板的can2接口是个tm4pin的，而c610电调出来的是2pin的，又要自己魔改一下线头了，nmd，wsm

好了我要开始认真的写云台了，但是在这之前还是有个小问题，就是射击任务和云台任务之间的关系到底如何处理

看了一晚上AHRS，表面懂了，挺有意思的，笔记都记在onenote上面了

# 2019/2/21

但是有一个问题，作为一个冷酷的jy901使用者，为什么我要关心AHRS解算呢（

好吧关心一下总是好的，毕竟ICRA的车就用板载9轴调的稳到亲妈不认

然后继续画我的整体算法图，把gimbal和shoot的遥控部分的代码写掉再说，我倒是不指望能够在这边能找到一个可以用来调的云台了

# 2019/3/23

车子来了，我要开始调了，虽然我很想很想把所有代码改成符合unix/linux风格的，但是现在实在是太不合时宜了，所以我还是就这样写吧，后面的人也不要对我这种风格突变感到奇怪

然后反正这版代码估计在拍完中期视频之后就可以推翻重写了，因为回头下供弹+滑环的版本需要双主控来弄，欸嘿嘿

# 2019/3/24

熬夜把 mpu的代码给移植了一下，但是还没有认真研究，这个代码让我觉得比较鬼的是好像他要自己算掉偏置？？？这么6p的吗？

今天白天测试一下能不能用先

看了一下能用，而且仿佛效果不错的样子，但具体还是得拿个东西测试一下数据（而且话说这完全没取消掉偏置吧

效果不错个大头鬼，数据不会动，看了一下发现陀螺仪角速度就不会动，我有点火大，再研究研究

结论就是spi读取了个假的，寄存器数据根本没被读出来，好了这下我还得慢慢研究是哪边出的问题

然后发现果然是device_init没调用，但是这个时候报的一个错让我很懵逼
```
../Src/main.c(129): warning:  #223-D: function "mpu_device_init" declared implicitly
```
尽管我应该正常的Include了头文件

extern了一下，上面的bug解决了，然而最大的问题也就是mpu读不到寄存器的问题仍然没解决，我有点怀疑是不是我IO口初始化的问题了

就是IO初始化的问题，数据成功读出来了，而且经过测试，pitch轴的数据读取非常的好，然而yaw轴却一直在飘，检查了一下发现问题在于磁力计的数据没有被正常读取出来，然后发现特别搞的一件事，我移植的这版代码完完全全没把磁力计的数据带进融合解算里面，这就让我感觉非常非常有趣，所以为了功能性考虑，回头我肯定是要重新移植一下ICRA那个工程里面的解算的，那个里面把磁力计数据算进了yaw轴解算里面，但是最近这段时间就算了，我先拿电机反馈的角度值当闭环吧，反正现在这个车子又不涉及无头模式，不需要高精度的yaw轴数据

# 2019/3/25

我要猝死了，先躺会，今天的目标是把云台的控制代码完成，考虑到bsp层我已经写好了，mpu的数据我也已经能读了，那么剩下的任务无非就是分成pitch轴和yaw轴各自的控制实现了

# 2019/3/28

开始写傻吊代码，IMU的pitch反馈给了gimbal_motor_pitch，先把几个反馈给过去再说吧，好烦啊（结果发现我自己一大早就是CAN接收中断把反馈的东西给写好了，不愧是我，工作效率真jb高） 

看了一下数据，不愧是垃圾编码器，电机速度反馈在0-16-（-32）几个值之间跳变，真的讨厌，我要考虑一下滤波或者换个东西来反馈吗？反正直接用大概率出事

看了一下ICRA的代码，反馈果然还是用的IMU，那就没什么好说的了

简单测试了一下，pitch轴角速度反馈很好看，yaw轴会有-4的偏置，所以手动把偏置给修正掉了，概括一下就是一句话——稳得一匹

看了一下任务执行时间，imu_task大概比gimbal_task慢了一个时间单位，考虑到1个时间单位挺小的了，所以基本上可以当成是同步更新的，欸嘿嘿
然后简单拿ICRA车子的例子来说一下云台的控制算法吧，简单来说就是这样——
```
while(1)
{
  pid_calc(&pid_yaw, gim.pid.yaw_angle_fdb, gim.pid.yaw_angle_ref);
  pid_calc(&pid_pit, gim.pid.pit_angle_fdb, gim.pid.pit_angle_ref);
  
  gim.pid.yaw_spd_ref = pid_yaw.out;
  gim.pid.pit_spd_ref = pid_pit.out;
  
  gim.pid.yaw_spd_fdb = gim.sensor.yaw_palstance;
  gim.pid.pit_spd_fdb = gim.sensor.pit_palstance;
  
  pid_calc(&pid_yaw_spd, gim.pid.yaw_spd_fdb, gim.pid.yaw_spd_ref);
  pid_calc(&pid_pit_spd, gim.pid.pit_spd_fdb, gim.pid.pit_spd_ref);

  if (gimbal_is_controllable())
  {
    glb_cur.gimbal_cur[0] = YAW_MOTO_POSITIVE_DIR*pid_yaw_spd.out;
    glb_cur.gimbal_cur[1] = PIT_MOTO_POSITIVE_DIR*pid_pit_spd.out;
    glb_cur.gimbal_cur[2] = pid_trigger_spd.out;
  }
  else
  {
    memset(glb_cur.gimbal_cur, 0, sizeof(glb_cur.gimbal_cur));
    gim.ctrl_mode = GIMBAL_RELAX;
    //pid_trigger.iout = 0;
  } 
}

//ICRA这破车代码牛逼之处在于安全性弄得特别特别好，能爆炸的地方都给削掉了
```
Pitch的控制代码写完了，之后要解决的问题只有两个，一个是如何把计算出来的pid转化成实际给电机的数值，另外一个是用什么方式控制遥控器的期望
然后对于yaw轴来说，麻烦稍微多一点，我要通过某种计算方式把电机的实际角度换算到和imu的数据同一个数量级的东西，然后再重复pitch轴的工作，好烦啊

转换个p，直接调参就好了

最后的参数：

位置环kp:8 ki:0 kd:0

速度环kp:21 ki:1.3e-06 kd:0

发现静差比较大
```
Motor_t Gimbal_Motor_Pitch  = {{{13.0f,0.000001f,0.0f},{10.0f,0.00019f,0.0f}}};
```
一个比较好的参数

yaw轴角度用了简单的数学方式标定了一下，把编码器转化成了实际角度值，然后就正常带进去写代码+调试就ok了，哪来那么多烦人的东西，完美

yaw轴弄完了

现在我死了

因为我发现这样一件好玩的事情，yaw轴单独调，很好，完美，pitch轴单独调，很好，完美，最后合在一起——boom  

绝了，我没想到系统之间会有相互干涉，我死了，艹

# 2019/3/29

发现如果不加上某个操作的话，遥控器离线是检测不了的，本来想着打算给几个驱动器设置个初值就解决问题的，后来发现不对，假如遥控器中途关了就还是哦豁完蛋

所以这个问题回头还是要好好想一下怎么解决（好烦啊

调了半天参数，发现还是很讨厌很讨厌，pitch轴的角度会受到yaw轴很明显的干扰，我思考了一下觉得我有必要给倒霉的pitch轴角度数据一个滤波，不给的话会爆炸，那么采用什么滤波方式呢？

1.移植神奇的icra的卡尔曼滤波
2.均值滤波解决问题
在思考良久之后，我觉得先来一个均值滤波弄一下看看能不能好一点（pitch轴的扰动真的是太大太恶心了）

# 2019/3/30

你妈的

我发现

我把

pitch轴的反馈和yaw轴的反馈弄反了

我杀我自己

然后发现IMU的角度反馈非常的屎，而且延迟程度非常严重，总的来说就是很爆炸，所以我直接换成了用编码器当反馈，现在这版云台效果ok了

然而新的问题发现，在我遥控器数据紧急变换的情况下，底盘会跑飞，让我来研究一下这个bug

结果发现只是底盘pid参数太大了而已，小问题

等一下我用遥控器去控制一下云台，现在pitch轴可以用遥控器控了，yaw轴我暂时没有特别大的兴趣

# 2019/3/31

今天心情好的话就把发射机构给弄完吧

（不对好像必须得头铁弄完的样子，还有给视觉留的接口）

发射机构倒是好弄，用s1控制一下模式就行了，控制逻辑用的是打到0开摩擦轮，打到2开拨轮，再打到0关摩擦轮，打到2关拨轮（好jb麻烦啊）

防卡弹和摩擦轮速度闭环这种烦人的事情弄不弄取决于我的心情（

然后发现比较讨厌的事情，需要调整一下代码结构，之前是把摩擦轮挂在CAN1上面的，但是现在的硬件结构搞得我好像是只能用CAN2来弄了，有点烦人

看了一下自己的傻逼代码，好像确实就没怎么考虑弄

现在这边来了个贼烦人的问题，这个傻逼2006电机根本不听我的话，左右摩擦轮倒是都好得很，就这个m2006最讨人厌，我先弄着看看到底是啥问题吧

结果发现是C610电调坏了,mdzz，回头看看要不要重新刷个固件啥的，这傻逼玩意浪费了我快一个小时的人生，真的烦人

# 2019/4/1

开始头铁发射机构控制代码，感觉其实没有什么特别好说的，在遥控模式下要实现的功能也很简单，用s1来作为整个电极的控制就ok了，没啥特别大的问题，这里可以先看一下ICRA的控制逻辑

呸，看个鬼的ICRA，这么简单的东西

打一次到上面，摩擦轮开，打两次摩擦轮关

打一次到下面，拨轮开，打两次拨轮关

然后我花了不少时间在把我的developing diary转成markdown，markdown真好玩，欸嘿嘿（实际上是摸鱼

本来打算把给视觉的接口给弄出来然后测试一下的，不过这会已经很晚了所以先咕咕咕

# 2019/4/2

嘤嘤嘤，今天再把键鼠给弄一下好像就得把车子给村里送过去了_(:3_|∠)_

反正之间接口啥的都留好了，直接写就ok了呗

不过话说回来考虑一下实际的控制场景，云台那边反倒是好像还会好写一点，直接将鼠标的绝对位置值和云台角度之间做一个映射就可以了，发射机构那边也不会太麻烦，一次右键开摩擦轮，一次右键关摩擦轮，一次左键开拨轮，一次左键关拨轮。真的要说的话其实应该写一下单发模式和连发模式的，不过这边工期这么紧急的情况下总之先咕咕咕了

然后是关于底盘的控制策略问题，总之先找辆车的控制逻辑来简单观摩一下：

不对，更优先的不是找个软件测试一下遥控器发送键鼠的情况然后标定吗

笑死我了，鼠标控制那个倒霉东西还是得增量式控制，回头我最好写一个云台带着底盘动的逻辑，目前先实现基本的带着动就行了

大力甩了鼠标，看了一下幅值能到500就很了不起了，我可以借此机会把这个垃圾东西标定一下

搞定了云台，灵敏度可以在sys_config.h里面调

然后我被一个该死的C语言问题锤爆了，那就是联合体怎么用的问题，因为我当初定义remote_data的时候用了一个联合体来装数据

另外底盘控制模式那边还有个问题，回头修一下

这样写了一下代码，结果发现了非常非常谜的事情

（本来这里应该有一段丢人的代码但是后来被我删了）

不愧是我，能写出这么傻逼的代码

功能正常，但是在暴力操作的情况下会跑飞，我知道你们不会好好看developing diary，所以我要在这里留下这段话，等到你们跑飞来看的时候无情的嘲讽你们哈哈哈哈哈哈哈哈哈

然后想要停下来的话，