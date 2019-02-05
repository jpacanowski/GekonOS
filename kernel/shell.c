#include <keyboard.h>
#include <video.h>
#include <stdint.h>
#include <shell.h>
#include <string.h>
#include <gui.h>

int cli_len;       // from kbd.c
char cli[256];     // from kbd.c

extern char *currentpath; // from main.c

void show_cmdline(void)
{
   cli_len=0;
   cli[0]='\0';
   SetColor(GREEN);
   kprintf("[root]# ");
   SetColor(LIGHT_GREY);
}

void lsh_loop(void)
{
   unsigned char key;
   show_cmdline();
   for(;;)
   {
     // loop if quere is empty
     if(kbd_keys_available())
       continue;
     key=kbd_get_key();
     // if kbd_get_key failed, key==0
     if(!key)
       continue;
     switch(key)
     {
       case '\b':
          if(cli_len)
          {
            cli_len--;
            kprintf("%c", key);
            kprintf(" ");
            kprintf("%c", key);
          }
          break;
       case '\n':
          /*if(console->csr_y >= 24)
          {
             scroll();
             console->csr_y = 24;
             console->csr_x = 0;
          }
          else*/ kprintf("\n");
          parse_cli();
          show_cmdline();
          break;
       case '\t':
          break;
       default:
          kprintf("%c", key);
          cli[cli_len++]=key;
          break;
     }
   }
}

int parse_cli(void)
{
        // We should do some fancy parameter parsing as well, since functions require them
        // (eg. del test.txt)
        cli[cli_len] = '\0'; // To terminate string

        if((!strcmp(cli, "help")) &&( cli_len==4))
        {
                kprintf("\n\t\t\tAvailable commands:\n");
                kprintf("\tcls \thalt \thelp \tinfo \tkill \tlist \tls\n");
                kprintf("\tmem \treboot \ttask \ttasks \ttime \ttop \tuptime\n");
                kprintf("\tver\n");
                kprintf("Other commands can only be executed if they are in the bin directory\n\n");
                return 1;
        }

        if((!strcmp(cli, "list")) &&(cli_len==4))
        {
                kprintf("Usage: list <group>\n");
                return 1;
        }

        if((!strcmp(cli, "list pci")) &&(cli_len==8))
        {
                //list_pci(); // from pci.c
                return 1;
        }

        if((!strcmp(cli, "time")) &&(cli_len==4))
        {
                //show_time();
                return 1;
        }

        if((!strcmp(cli, "uptime")) &&(cli_len==6))
        {
                //show_uptime();
                return 1;
        }

        if((!strcmp(cli, "mem")) &&(cli_len==3))
        {
                // int total_ram;
                // total_ram = mm_count();
                // printf("%d MB total memory\n",total_ram);
                return 1;
        }

        if((!strcmp(cli, "dump")) &&(cli_len==4))
        {
                uint32_t eax,ebx,ecx,edx,edi,esi,ebp,esp;
                __asm__ __volatile__("movl %%eax, %0" : "=r"(eax));
                __asm__ __volatile__("movl %%ebx, %0" : "=r"(ebx));
                __asm__ __volatile__("movl %%ecx, %0" : "=r"(ecx));
                __asm__ __volatile__("movl %%edx, %0" : "=r"(edx));
                __asm__ __volatile__("movl %%edi, %0" : "=r"(edi));
                __asm__ __volatile__("movl %%esi, %0" : "=r"(esi));
                __asm__ __volatile__("movl %%ebp, %0" : "=r"(ebp));
                __asm__ __volatile__("movl %%esp, %0" : "=r"(esp));
                kprintf("\tEAX=%08X   EBX=%08X   ECX=%08X   EDX=%08X\n",
                  eax,ebx,ecx,edx);
                kprintf("\tEDI=%08X   ESI=%08X   EBP=%08X   ESP=%08X\n",
                  edi,esi,ebp,esp);
        }

        if((!strcmp(cli, "cpuinfo")) &&(cli_len==7))
        {
                //showProcessorInformation();
                return 1;
        }

        if((!strcmp(cli, "cls")) &&(cli_len==3))
        {
                //clrscr();
                //set_console();
                return 1;
        }

        if((!strcmp(cli, "halt")) &&(cli_len==4))
        {
                kprintf("stopping interrupts...");
                __asm__("cli");
                kprintf("Ok.\n");
                kprintf("system halted!\n");
                __asm__("hlt");
                return 1;
        }

        if((!strcmp(cli, "reboot")) &&(cli_len==6))
        {
                //ProcReboot();
                return 1;
        }

        if((!strcmp(cli, "shutdown")) &&(cli_len==8))
        {
                //PowerOff();
                return 1;
        }

        if((!strcmp(cli, "gui")) &&(cli_len==3))
        {
                GUI_Init();
                return 1;
        }

        if((!strcmp(cli, "ls")) &&(cli_len==2))
        {
                //DisplayCurrentDir(1);
                return 1;
        }

        // if((!strcmp(cli, "tasks")) &&(cli_len==5))
        // {
        //         int ts;
        //         ts = add_task((unsigned)demotask, "root", PR_NORMAL, "demotask");
        //         _tasks[ts].status = TS_RUNNABLE;
        //         ts = add_task((unsigned)demotask, "root", PR_NORMAL, "demotask");
        //         _tasks[ts].status = TS_RUNNABLE;
        //         ts = add_task((unsigned)demotask, "root", PR_NORMAL, "demotask");
        //         _tasks[ts].status = TS_RUNNABLE;
        //         ts = add_task((unsigned)demotask, "root", PR_NORMAL, "demotask");
        //         _tasks[ts].status = TS_RUNNABLE;
        //         ts = add_task((unsigned)demotask, "root", PR_NORMAL, "demotask");
        //         _tasks[ts].status = TS_RUNNABLE;
        //         printf("\n5 tasks added");
        //         return 1;
        // }

        // if((!strcmp(cli, "kill 1")) &&(cli_len==6))
        // {
        //         _tasks[1].status = TS_BLOCKED;
        //         sysprintf("        ", 49, 0, 0x07);
        //         return 1;
        // }

        if((!strcmp(cli, "task")) &&(cli_len==4))
        {
                kprintf("Add a demo task.\nUsage: task <pid>\n");
                return 1;
        }

        if((!strcmp(cli, "kill")) &&(cli_len==4))
        {
                kprintf("Usage: kill <pid>\n");
                return 1;
        }

        if((!strcmp(cli, "top")) &&(cli_len==3))
        {
                //shell_top();
                return 1;
        }

        if((!strcmp(cli, "ver")) &&(cli_len==3))
        {
                //printf("LizardOS kernel-%s %s\n", KERNEL_VERSION, KERNEL_DATE);
                return 1;
        }

        // if((!strcmp(cli, "info")) &&(cli_len==4))
        // {
        //         printf("###########################\n");
        //         printf("#    :: Lizard1982 ::     #\n");
        //         printf("#    lizard1982@wp.pl     #\n");
        //         printf("#   +48508695686 (SMS)    #\n");
        //         printf("#  www.lizard1982.prv.pl  #\n");
        //         printf("###########################\n");
        //         return 1;
        // }

        return 0;
}
