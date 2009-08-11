/*
Copyright (c) 2007-2009 by Juliusz Chroboczek

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

extern const unsigned char v4prefix[16];

/* This holds both IPv6 and IPv4, and both prefixes and addresses.  For
   IPv4, we use v4prefix above.  For addresses, plen is 0xFF. */

struct prefix {
    unsigned char p[16];
    unsigned char plen;
};

#define MAX_PREFIX 8

struct prefix_list {
    int n;
    struct prefix l[MAX_PREFIX];
};

/* This is used both to hold the contents of a message and to hold our
   current configuration. */
struct config_data {
    /* The following fields come from a message */
    unsigned origin, origin_m, expires, expires_m;
    struct prefix_list *server_ipv6, *server_ipv4;
    struct prefix_list *ipv6_prefix, *ipv4_prefix,
        *ipv6_address, *ipv4_address;
    struct prefix_list *name_server, *ntp_server;
    /* This field is only in our configuration. */
    struct prefix_list *our_ipv6_address;
};

extern struct config_data *config_data;

void prefix_list_extract4(unsigned char *dest, struct prefix_list *p);
void prefix_list_extract6(unsigned char *dest, struct prefix_list *p);
unsigned int config_renew_time(void);
void free_config_data(struct config_data *config);
int config_data_compatible(struct config_data *config1,
                           struct config_data *config2);
struct config_data *copy_config_data(struct config_data *config);
struct config_data *make_config_data(int expires,
                                     unsigned char *ipv4,
                                     unsigned char *ipv6_prefix,
                                     unsigned char *name_server,
                                     int name_server_len,
                                     unsigned char *ntp_server,
                                     int ntp_server_len,
                                     char **interfaces);

struct config_data *parse_message(int configure,
                                  const unsigned char *data, int len,
                                  char **interfaces);
int unconfigure(char **interfaces);
int query_body(unsigned char opcode, int time, const unsigned char *ipv4,
               unsigned char *buf, int buflen);
int server_body(unsigned char opcode, struct config_data *config,
                unsigned char *buf, int buflen);
int address_conflict(struct prefix_list *a, struct prefix_list *b);
int if_eui64(char *ifname, unsigned char *eui);
int random_eui64(unsigned char *eui);
