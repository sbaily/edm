#ifndef __ipnis_h
#define __ipnis_h 1

#define IPNIS_MAX_SERVICE_NAME	31

int ipnis_get_service (
  int num_connect_fails,
  const char *generic_service_name,
  char *service_name,
  char *node_name,
  size_t maxName,
  int *mode
);

globalvalue IPNIS_SUCCESS;
globalvalue IPNIS_UNKSVC;

#endif
