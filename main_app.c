/* 
 * File:   main_application.c
 * Author: mohammed emad
 *
 * Created on 08 August 2023, 22:40
 */

#include "main_app.h"
/*lcd1 and lcd2 definition in euc_layer_init*/
extern lcd_8pin lcd1;
extern keypad keypad1;
STD_ReturnType ret=E_NOT_OK;
STD_ReturnType ret2=E_NOT_OK;
uint_8 str[4];
uint_8 number=0;
uint_8 number_1=0;
uint_8 value_1=0;
uint_8 value_2=0;
uint_8 result=0;
int main() {
    uint_8 index=0,counter=0;
    ret=gpio_init();
    ret=begin_loading();
    ret=lcd_8bit_send_command(&lcd1,DISPLAY_ON_CURSOR_OFF_BLINKING_OFF);
    while(1)
    {
        ret2=keypad_get_value(&keypad1,&number);
       if((('+'!=number)&&('*'!=number)&&('/'!=number)&&('-'!=number)&&('='!=number)&&('#'!=number))&&(ret2==E_OK))
        {
            ret=lcd_8bit_send_char_data(&lcd1,number);
            if(counter==0)
            {
                value_1=convert_char_into_integer(number);
                counter=1;
            }
            else if(counter==1)
            {
                value_2=convert_char_into_integer(number);
                counter=0;
            }
            ret2=E_NOT_OK;
            __delay_ms(250);
        }
        else if((('+'==number)||('*'==number)||('/'==number)||('-'==number)||('='==number)&&(number!='#'))&&(E_OK==ret2))
        {
            if((('+'==number)||('*'==number)||('/'==number)||('-'==number)))
            {
                number_1=number;
            }
            switch(number)
            {
                case '+':
                    ret=lcd_8bit_send_char_data(&lcd1,'+');
                    break;
                case '-':
                    ret=lcd_8bit_send_char_data(&lcd1,'-');
                    break;
                case '*':
                    ret=lcd_8bit_send_char_data(&lcd1,'*');
                    break;
                case '/':
                    ret=lcd_8bit_send_char_data(&lcd1,'/');
                    break;
                case '=':
                {
                    ret=lcd_8bit_send_char_data(&lcd1,'=');
                    ret=lcd_8bit_send_string_pos(&lcd1,2,1,"Result:");  
                    switch(number_1)
                    {
                        case '+':
                            result=value_1+value_2;
                            print_result(result);
                            break;
                        case '-':
                            result=value_1-value_2;
                            print_result(result);
                            break;
                        case '*':
                            result=value_1*value_2;
                            print_result(result);
                            break;
                        case '/':
                        {
                            if(value_2==0)
                            {
                                ret=lcd_8bit_send_string_pos(&lcd1,2,10,"error !");
                                ret=lcd_8bit_send_string_pos(&lcd1,3,1,"cant divide by zero");
                            }
                            else
                            {
                                
                                result=value_1/value_2;
                                print_result(result);   
                            }
                            break;
                        }      
                    }
                    break;
                }
                          
            }
            ret2=E_NOT_OK;
            __delay_ms(250);
        }
        else if((number=='#')&&(E_OK==ret2))
        {
            ret=lcd_8bit_send_command(&lcd1,CLEAR_DISPLAY);
            ret=lcd_8bit_send_command(&lcd1,RETURN_HOME) ; 
            ret2=E_NOT_OK;
            __delay_ms(200);
        }
        else{/*noting*/}
        
    }
    return (EXIT_SUCCESS);     // press ctrl and left click
}
STD_ReturnType gpio_init()
{
    STD_ReturnType ret=E_OK;
    ret=euc_init();
    return ret;
}

STD_ReturnType begin_loading(void)
{
    uint_8 index0=0;
    ret=lcd_8bit_send_command(&lcd1,DISPLAY_ON_CURSOR_OFF_BLINKING_OFF);
    uint_8 counter=0,counter2=0;
        ret=lcd_8bit_send_string_pos(&lcd1,2,6,"loading");
        for(counter=0;counter<3;counter++)
        {
            for(counter2=0;counter2<5;counter2++)
            {
                ret=lcd_8bit_send_char_data_pos(&lcd1,2,13+counter2,'.');
                __delay_ms(300);
            }
            for(counter2=0;counter2<5;counter2++)
            {
                ret=lcd_8bit_send_char_data_pos(&lcd1,2,13+counter2,' ');
                
            }
            __delay_ms(300);
        }
        
        ret=lcd_8bit_send_command(&lcd1,CLEAR_DISPLAY);
        ret=lcd_8bit_send_string_pos(&lcd1,1,7,"HI THERE");
        ret=lcd_8bit_send_string_pos(&lcd1,2,2,"I AM READY TO WORK ");
        __delay_ms(2000);
        ret=lcd_8bit_send_command(&lcd1,CLEAR_DISPLAY);
       
}
uint_8 convert_char_into_integer(uint_8 character)
{
    uint_8 number=0;
    number = atoi(&character);
    return number;
}

STD_ReturnType print_result(uint_8 result)
{
    STD_ReturnType ret=E_OK;
    ret=convert_byte_to_string(result,str);
    ret=lcd_8bit_send_string_pos(&lcd1,2,8,str); 
    ret=lcd_8bit_send_command(&lcd1,RETURN_HOME) ; 
    return ret;
}