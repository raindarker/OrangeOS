#ifndef __ORANGE_PROTO_H__
#define __ORANGE_PROTO_H__

void out_byte(u16 port, u8 value);
u8 in_byte(u16 port);
void disp_str(char * info);
void disp_color_str(char * info, int color);
void init_prot();
void init_8259A();

#endif /* __ORANGE_PROTO_H__ */
