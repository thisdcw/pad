
#include "key.h"
#include "timer.h"

// ----------------------------------- key_driver --------------------------
unsigned char key_driver(void)
{
    static unsigned char key_state = 0;
    static unsigned int key_time = 0;
    static unsigned char key_return_buffer=0;
    unsigned char key_press, key_return;

    key_return = N_KEY;                         // 清除 返回按键值
    
	key_press |= KEY_MASK;
    key_press = P3&KEY_MASK;                    // 读取当前键值
	
    switch (key_state)
    {
        case KEY_STATE_0:                       // 按键状态0：判断有无按键按下
            if (key_press!=KEY_MASK)                      // 有按键按下
            {
                key_time = 0;                   // 清零时间间隔计数
                key_state = KEY_STATE_1;        // 然后进入 按键状态1
            }
            break;

        case KEY_STATE_1:                       // 按键状态1：软件消抖（确定按键是否有效，而不是误触）。按键有效的定义：按键持续按下超过设定的消抖时间。
            if (key_press!=KEY_MASK)
            {
                key_time++;                     // 一次10ms
                if (key_time >= SINGLE_KEY_TIME)// 消抖时间为：SINGLE_KEY_TIME*10ms = 50ms;
                {
                    key_state = KEY_STATE_3;    
					if(key_press==KEY_ON_OFF){
						key_return=KEY_ON_OFF_SHORT;
                    	//key_state = KEY_STATE_2;//按键有效包括两种：单击或者长按，进入 按键状态2， 继续判定到底是那种有效按键
                    }else if(key_press==KEY_UP){
                    	key_return=KEY_UP_SIGLE;
                    }else if(key_press==KEY_DOWN){
                    	key_return=KEY_DOWN_SIGLE;
                    }
                }
            }
            else
                key_state = KEY_STATE_0;        // 如果按键时间没有超过，判定为误触，按键无效，返回 按键状态0，继续等待按键
            break;

        case KEY_STATE_2:                       // 按键状态2：判定按键有效的种类：是单击，还是长按
            if (key_press==KEY_MASK)                       // 如果按键在 设定的长按时间 内释放，则判定为单击
            {
                key_state = KEY_STATE_0;        // 返回 按键状态0，继续等待按键
				key_return=key_return_buffer;
            }
            else
            {
                key_time++;

                if (key_time >= LONG_KEY_TIME)  // 如果按键时间超过 设定的长按时间（LONG_KEY_TIME*10ms=120*10ms=1200ms）, 则判定为 长按
                {
                    key_state = KEY_STATE_3;    // 去状态3，等待按键释放
                    key_return=KEY_ON_OFF_LONG; 
                    key_time=0;  
                }
            }
            break;

        case KEY_STATE_3:                       // 等待按键释放
            if (key_press==KEY_MASK)
            {
                key_state = KEY_STATE_0;        // 按键释放后，进入 按键状态0 ，进行下一次按键的判定
            }
            break;
            
        case KEY_STATE_4:
        	
			break;
        default:                                // 特殊情况：key_state是其他值得情况，清零key_state。这种情况一般出现在 没有初始化key_state，第一次执行这个函数的时候
            key_state = KEY_STATE_0;
            break;
    }

    return key_return;                          // 返回 按键值
}

unsigned char key_read(void)
{
    //static unsigned char key_state1 = 0, key_time1 = 0;
    unsigned char key_return, key_temp;
	
	if(t10msFlag1){
		t10msFlag1=0;
	    key_return = N_KEY;                          // 清零 返回按键值
	    key_temp = key_driver();                     // 读取键值
		key_return = key_temp;                       // 返回 按键值
	}else{
    	key_return = N_KEY;                          // 清零 返回按键值
    }
    
    return key_return;                               // 返回 按键值
    
}