/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 21:38:30 by ldedier           #+#    #+#             */
/*   Updated: 2018/08/22 21:29:23 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

UDPpacket	*ft_create_packet(size_t packetSize)
{
	UDPpacket *packet;

	if (!(packet = SDLNet_AllocPacket(packetSize)))
		ft_error("alloc_packet_failed");
	printf("%zu\n", packetSize);
	return (packet);
}

int		ft_open_port(UDPsocket *socket, int port)
{
	*socket = SDLNet_UDP_Open(port); // 0 => take any available port
	if (socket == NULL)
	{
		printf("coudln't open socket %d\n", port);
		return (0);
	}
	return (1);
}

int		ft_resolve_ip_address(IPaddress *address, char *ip, int port)
{
	if (SDLNet_ResolveHost(address, ip, port) == -1)
		return (0);
	return (1);
}

int		ft_init_server_bundle(t_server_bundle *bundle, size_t size)
{
	if (!(bundle->packet = ft_create_packet(size)))
		return (0);
	if (!(bundle->message = (t_server_message *)malloc(size)))
		return(0);
	return (1);
}

int		ft_init_client_bundle(t_client_bundle *bundle, size_t size)
{
	if (!(bundle->packet = ft_create_packet(size)))
		return (0);
	if (!(bundle->message = (t_client_message *)malloc(size)))
		return(0);
	return (1);
}
