
#ifndef AUX_H
#define AUX_H

#include <stddef.h>
#include "fsm.h"

/* One event in the washing machine history. */
typedef struct {
    size_t tick;
    washer_state_t state;
} aux_event_t;

/* Opaque event history stack. */
typedef struct aux aux_t;

/* Create and destroy the stack. */
aux_t *aux_create(void);
void aux_destroy(aux_t *stack);

/* Stack operations. */
int aux_push(aux_t *stack, aux_event_t event);
int aux_pop(aux_t *stack, aux_event_t *event);
int aux_peek(const aux_t *stack, aux_event_t *event);

/* Stack information. */
size_t aux_count(const aux_t *stack);
int aux_is_empty(const aux_t *stack);

#endif
