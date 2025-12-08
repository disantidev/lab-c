#include <stdio.h>
#include <pwd.h>

int main (void)
{
	struct passwd *ptr;

	setpwent();

	while ((ptr = getpwent()) != NULL) {
		if (ptr->pw_name[0] != '_')
			printf("%s\n", ptr->pw_name);
	}

	endpwent();

	return 0;
}
