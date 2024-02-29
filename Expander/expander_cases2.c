
#include "../minishell.h"

//THIS FUNCTION FINDS HOW MUCH LEN SHOULD WE ADD TO THE NEW
//STRING AND HOW MUCH LEN WE SHOULD SUBTRACT. SO IF I HAVE
//A [$USER] IN MY STRING AND USER=MR.MOSES. THEN I NEED TO ADD
//THE LEN OF MR.MOSES WHICH IS 8 AND SUBTRACT THE LEN OF $USER 
//WHICH IS 5. SO THE DIFF LEN IS 8 - 5 = 3. MEANING WE NEED
//TO MALLOC THREE MORE CHARACTERS. IF THE DIFF LEN IS NEGATIVE
//WHICH IS TOTALLY POSSIBLE, WE MALLOC FEWER CHARACTERS THAN THE
//ORIGINAL STRING.
//sub_len = i plus 1 because we want to delete the $
static int	get_diff_len(char *env)
{
	int	sub_len;
	int	add_len;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (env[i] && (env[i] != '='))
		i++;
	sub_len = i + 1;
	i++;
	while (env[i + j])
		j++;
	add_len = j;
	return (add_len - sub_len);
}

//THIS FUNCTION DETECTS WHETHER THE TRAILOR OF THE ${ IS
//AVAILABLE IN THE ENVAR LIST OR NOT. IF IT IS, IT GOES
//OVER TO COPIER, OTHERWISE IT RETURNS NULL.
char	*detector(char *str, int s_ind, char *env[])
{
	char	*buffer;
	int		i;
	int		diff_len;

	i = 0;
	while (env[i])
	{
		if (in_env_string(str + s_ind, env[i]))
		{
			diff_len = get_diff_len(env[i]);
			buffer = (char *)malloc(ft_strlen(str) + (diff_len) + 1);
			if (!buffer)
				return (warn_message(), NULL);
			copier(buffer, str, env[i]);
			return (buffer);
		}
		i++;
	}
	return (NULL);
}
