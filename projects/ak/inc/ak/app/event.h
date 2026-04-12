#ifndef ak_app_event_h
#define ak_app_event_h

typedef enum
{
  ak_event_none,
} ak_event_type;
typedef struct
{
  ak_event_type type;
} ak_event;

#endif
