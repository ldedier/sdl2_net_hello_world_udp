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
	return (packet);
}

t_packet_vector	ft_create_packet_vector(size_t data_size)
{
	t_packet_vector res;

	UDPpacket **packets;
	res.nb_packets = ceil(data_size / (double)MAX_UDP_PACKET_SIZE);
	if (!(res.packets = SDLNet_AllocPacketV(res.nb_packets, MAX_UDP_PACKET_SIZE)))
		ft_error("alloc_vector_failed");
	return (res);
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
	printf("%zu\n",  size);
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

void	ft_memcpy_to_packet_vector(void *source, UDPpacket **packet_vector, size_t size)
{
	int i;
	int copied_bytes;
	int	bytes_to_copy;

	i = 0;
	copied_bytes = 0;
	while (packet_vector[i])
	{
		bytes_to_copy =  ft_min(size - copied_bytes, packet_vector[i]->maxlen);
		memcpy(packet_vector[i]->data, &(source[copied_bytes]), bytes_to_copy);
		packet_vector[i]->len = bytes_to_copy;
		copied_bytes += bytes_to_copy;
		i++;
	}
	printf("copied %d bytes out of %zu in %d packets\n", copied_bytes, size, i);
}

void	ft_memcpy_from_packet_vector(void *dest, UDPpacket **packet_vector)
{
	int i;
	int	copied_bytes;

	i = 0;
	copied_bytes = 0;
	while (packet_vector[i])
	{
		printf("%d\n", packet_vector[i]->len);;
		memcpy(&(dest[copied_bytes]), packet_vector[i]->data, packet_vector[i]->len);
		copied_bytes += packet_vector[i]->len;
		i++;
	}
	printf("copied in total %d bytes through %d packets\n", copied_bytes, i);
}

void	ft_address_packets(UDPpacket **packet_vector, IPaddress address)
{
	int i;

	i = 0;
	while (packet_vector[i])
	{
		packet_vector[i]->address = address;
		i++;
	}
}
