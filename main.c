/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 16:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/19 10:19:34 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./inc/minirt.h"

/*******************************************************************************
******************************************************************************/
// <!> - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  </!>
///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************

			░█████╗░  ███████╗░█████╗░██╗██████╗░███████╗
			██╔══██╗  ██╔════╝██╔══██╗██║██╔══██╗██╔════╝
			███████║  █████╗░░███████║██║██████╔╝█████╗░░
			██╔══██║  ██╔══╝░░██╔══██║██║██╔══██╗██╔══╝░░
			██║░░██║  ██║░░░░░██║░░██║██║██║░░██║███████╗
			╚═╝░░╚═╝  ╚═╝░░░░░╚═╝░░╚═╝╚═╝╚═╝░░╚═╝╚══════╝

mlx_hook(window,  ConfigureNotify, StructureNotifyMask, handle_resize, data);

    ConfigureNotify: Triggered when the window is resized or moved.
    StructureNotifyMask: Ensures you are notified about structural changes, including resizing.
    handle_resize: Your custom callback function to handle the resize.


	partial transparence in sprite > xpm support transparence > pixel effective transparence
******************************************************************************/
///////////////////////////////////////////////////////////////////////////////]
// int	g_signalreceived;

int	main(int ac, char **av)
{
	t_data	data;

	initialization(ac, av, &data);

	ft_render_frame(&data, 1);
	mlx_loop(data.mlx);

	end(&data, 0, 1);
	return (0);
}
