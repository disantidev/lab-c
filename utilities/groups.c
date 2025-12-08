#include <stdio.h>
#include <grp.h>

int main (void)
{
	struct group *grp;

	setgrent();

	while ((grp = getgrent()) != NULL)
	{
		if (grp->gr_name[0] != '_')
			printf("%s\n", grp->gr_name);
	}

	endgrent();

	return 0;
}

