#include <linux/module.h>	// Needed by all modules
#include <linux/kernel.h>	// KERN_INFO
#include <linux/mm_types.h>	// include all mm type
#include <linux/sched.h>	// for_each_process, pr_info
#include <asm/page.h>

//Questions
// 1. is p already allocated and is it a valid place?
// 2. if true, what is start and end addresses of virtual memory area
// 3. is that virtual memory area readable, writable, and/or executable

int readAddr(void)
{	
	struct task_struct* task_list;
	struct mm_struct* mm_list;
	struct vm_area_struct* vm_list;
	int process_id = 5;
	size_t process_counter = 0; 
	
	//Test the function with p
	//struct task_struct* p= current;

	for_each_process(task_list)
	{
		if (task_list->pid == process_id)
		{
			//if((vm_list->vm_mm) == p)
			//{
			//pr_info("Vm located");
			pr_info("== %s [%d]\n",task_list->comm, task_list->pid);
			++process_counter;
			mm_list = task_list->mm;
			if (mm_list != NULL)
			{
				unsigned long flags = vm_list->vm_flags;
				if(flags & VM_READ)
				{
					pr_info("Start vm address [%lu]",vm_list->vm_start);
					pr_info("End vm address [%lu]",vm_list->vm_end);
					pr_info("VM_READ [%lu]", VM_READ);
					pr_info("vm_flags [%lu]", vm_list->vm_flags);
				}
			}
			//}
		}
	}
	return 0;
}

void cleanup_module(void)
{
        printk(KERN_INFO "== CLEANUP ]\n");
}

MODULE_LICENSE("MIT");
