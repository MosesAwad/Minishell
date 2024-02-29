/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   3-command_level.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moses <moses@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 01:50:10 by moses             #+#    #+#             */
/*   Updated: 2024/02/09 01:50:10 by moses            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//The point of this function is to rearrange the rdrlists from the
//command node (rdrlist1) and the rdrlists from the smplcmdnode node
//(rdrlist2) in a way such that the command node has all its arguments
//to the left and all its redirections to the right. As of now, the
//rdrlist2 is appended to the smplcmdnode but rdrlist1 is just floating.
//So what this function does is it loops through rdrlist1 till its end
//then attaches rdrlist2 at the end of rdrlist1, so that the files appear
//in the order in which they did in the commandline. If we did not have
//a redirlist1, then the smplcmdnode is ready as it is so we just return
//it.
t_ASTree	*case_checker1(t_ASTree *rdrlist1, t_ASTree *rdrlist2,
		t_ASTree *smplcmdnode)
{
	t_ASTree	*result;
	t_ASTree	*save_rdr1;

	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (warn_message(), NULL);
	save_rdr1 = rdrlist1;
	set_node_type(result, NODE_RDRGEN);
	set_node_data(result, NULL);
	if (rdrlist1)
	{
		if (rdrlist2)
		{
			while (rdrlist1->left)
				rdrlist1 = rdrlist1->left;
			rdrlist1->left = rdrlist2;
			rdrlist1 = save_rdr1;
		}
		attach_branch(result, smplcmdnode, rdrlist1);
	}
	else
		return (free(result), smplcmdnode);
	return (result);
}

//this handles cases where rdrlist1 is NULL but there still could be an
//rdrlist2 (rdlist2 is the one that came with the arg node of our command)
t_ASTree	*case_checker2(t_ASTree *rdrlist2, t_ASTree *smplcmdnode)
{
	t_ASTree	*result;

	result = (t_ASTree *)malloc(sizeof(*result));
	if (!result)
		return (warn_message(), NULL);
	if (rdrlist2)
	{
		set_node_type(result, NODE_RDRGEN);
		set_node_data(result, NULL);
		attach_branch(result, smplcmdnode, rdrlist2);
	}
	else
		return (free(result), smplcmdnode);
	return (result);
}

//just a small function to deal with stupid norminette and reduce
//the size of the function below, the command1() function. Basically,
//the way I designed the code is I append the redirection list of
//the command node to its right and all its tokens are to the left.
//So, if (smplcmdnode->right) then that means we do have a rdrlist
//stored in there. Thus, we assign copy it to rdrlist2 and clear the
//smplcmdnode's right node to just have its arguments attached to it.
t_ASTree	*assign_rdrlist2(t_ASTree *smplcmdnode)
{
	t_ASTree	*rdrlist2;

	rdrlist2 = copy_node(smplcmdnode->right);
	delete_node(smplcmdnode->right);
	smplcmdnode->right = NULL;
	return (rdrlist2);
}
