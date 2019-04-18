/*
 * Jessica Hilario 013944279
 * Joel Rodriguez 015222816
 * Assignment 3
 * Due: April 18, 2019
 * This program takes an address from and validates
 * that it is an allocated address within the virtual
 * memory area. Once it captures that it is true, it
 * then prints out the starting address and ending 
 * address of the virtual memory address as well if 
 * it is a readable, writable, and/or executable
 * file.
 *
 */
#include <linux/module.h>	// Needed by all modules
#include <linux/kernel.h>	// KERN_INFO
#include <linux/mm_types.h>	// include all mm type
#include <linux/sched.h>	// for_each_process, pr_info
#include <linux/mm.h>		// Flags

//Prints the start and end address of the found vm struct.
//Will also print out the flags (in digits) of the vm 
//struct as well as what type of flags it has.
void print_info(struct vm_area_struct* temp)
{
	//Get the flags of the vm struct
	unsigned long flags = temp->vm_flags;
	//Checks whether the flag is readable, writable, and/or executable
	if(flags & VM_READ & VM_WRITE & VM_EXEC)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No READ, WRTIE, EXEC problem!");
		pr_info("\n");					
	}
	else if(flags & VM_READ & VM_WRITE)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No READ, WRTIE problem!");
		pr_info("\n");
	}
	else if(flags & VM_READ & VM_EXEC)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No READ, EXEC problem!");
		pr_info("\n");
	}
	else if(flags & VM_WRITE & VM_EXEC)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No WRTIE, EXEC problem!");
		pr_info("\n");
	}
	else if(flags & VM_READ)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No READ problem!");
		pr_info("\n");
	}
	else if(flags & VM_WRITE)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No WRTIE problem!");
		pr_info("\n");
	}
	else if(flags & VM_EXEC)
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==No EXEC problem!");
		pr_info("\n");
	}
	else
	{
		pr_info("Start vm address [%lu]",temp->vm_start);
		pr_info("End vm address [%lu]",temp->vm_end);
		pr_info("vm_flags [%lu]", flags);
		pr_info("==There's an EXEC, READ, and WRITE problem!");
		pr_info("\n");
	}
}

//Reads the address passed through to see if it is valid
int readAddr(void *p)
{	
	struct task_struct* task_list;
	struct mm_struct* mm_list;
	struct vm_area_struct* vm_list;
	
	//Captures whether it is or isn't found in the vm struct	
	size_t flag = 0;

	//Loops through all the task_list
	for_each_process(task_list)
	{
		//Gets the mm_list from the task_list mm
		mm_list = task_list->mm;
		//If the mm_list has vm structs
		if (mm_list != NULL)
		{
			//Get the first vm struct from mm_list mmap
			vm_list = mm_list->mmap;
			//Loop through all the vm area structs
			do {
				//If the address is between the start and end address 
				//of the vm_list, than that address is allocated
				if((vm_list->vm_start >= p) & (p<=vm_list->vm_end))		
				{
					pr_info("Found");
					//Print the found address information
					print_info(vm_list);
					flag = 1;
					//Exit out of the while loop
					goto END;
				}
				//Go to the next vm list if it is not found
				vm_list = vm_list->vm_next;
			} while(vm_list != NULL);
				
		}
	}
	
	END:if (flag==0) pr_info("Address not found");
	return 0;
}

int init_module(void)
{
        printk(KERN_INFO "[ INIT ==\n");
	//Tester address
	unsigned long addr = 140511734895555;
        readAddr(addr);

        return 0;
}

void cleanup_module(void)
{
        printk(KERN_INFO "== CLEANUP ]\n");
}

MODULE_LICENSE("MIT");
