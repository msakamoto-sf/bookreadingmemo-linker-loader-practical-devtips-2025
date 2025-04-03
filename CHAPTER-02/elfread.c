#include <stdio.h>
#include <unistd.h>
#include <elf.h>

int main()
{
    void *p = &main;
    printf("address of main() = [%p]\n", p);
    int pgsize = getpagesize();
    printf("getpagesize(2) =[0x%02x]\n", pgsize);
    unsigned long long remain = (unsigned long long) p % pgsize;
    printf("address of main() %% 4KB page = [0x%02llx]\n", remain);
    /* for this program:
     * p - remain => 2nd "LOAD" segument address
     * p - remain - pgsize => subtract "INTERP" segument (=pagesize at this program)
     */
    p = p - remain - pgsize; // => head of ELF header region
    printf("nearest %p\n", p);
    Elf64_Ehdr *ehdr = (Elf64_Ehdr*)p;
    printf("heading 4 byte in ELF header = [0x%02x%c%c%c]\n",
        ehdr->e_ident[EI_MAG0],
        ehdr->e_ident[EI_MAG1],
        ehdr->e_ident[EI_MAG2],
        ehdr->e_ident[EI_MAG3]);
    return 0;
}