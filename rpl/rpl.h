#ifndef _RPL_H_
#define _RPL_H_

#include <stdint.h>
#include <stdbool.h>

#include "../util/list.h"
#include "../util/defs.h"
#include "../graphs/graphs.h"

#define RPL_MRHOF                 1
#define RPL_TAMU                  2

// From OpenWSN
#define MAXDAGRANK                0xffff
#define DEFAULTDAGRANK            MAXDAGRANK
#define MINHOPRANKINCREASE        256  //default value in RPL and Minimal 6TiSCH draft
#define DEFAULTLINKCOST           15

#define DIO_PERIOD_MS             30000 // 30 seconds
#define KA_PERIOD_MS              10000 // 10 seconds

#define N_TIMESLOTS_PER_DIO       DIO_PERIOD_MS / 10    // timeslots of 10 ms
#define N_TIMESLOTS_PER_KA        KA_PERIOD_MS / 10     // timeslots of 10 ms

#define STABLE_NEIGHBOR_THRESHOLD 50

typedef enum {
    MRHOF_ETX,
    TAMU,
} RPL_OF;

typedef struct rpl_neighbor_t {
	uint16_t     id;                 /* Node ID */

    uint16_t    rx_success;
    uint16_t    rx_failed;

    float       estimated_etx;

    uint16_t    dagRank;
    uint8_t     hop_count;

    bool        prefered_parent;
    bool        stable;             /* If PRR is above a threshold */

/* TAMU_RPL */
    bool        already_sampled;    /* Check if this neighbor was already sampled or not */
    double      beta_sample;

} RPL_Neighbor_t;

int execute_rpl(uint8_t rpl_alg, List *nodesList, Tree_t *tree, uint8_t sink_id, uint8_t channel, char *prr_file_prefix, uint32_t n_timeslots_per_file, uint32_t min_asn_per_dio, uint32_t min_asn_per_ka);
void init_rpl(List *nodesList, uint8_t sink_id);

void rplTXDIO(uint8_t rpl_alg, Node_t *dioNode, List *nodesList, uint8_t prrMatrix[][MAX_NODES][NUM_CHANNELS], uint8_t freq, uint64_t cur_asn, List *dio_to_transmit, List *ka_to_transmit, uint32_t min_asn_per_dio, uint32_t min_asn_per_ka);
void rplRxDIO(uint8_t rpl_alg, Node_t *rxNode, Node_t *txNode, uint8_t prr);
void rplTXKA(uint8_t rpl_alg, Node_t *kaNode, List *nodesList, uint8_t prrMatrix[][MAX_NODES][NUM_CHANNELS], uint8_t freq, uint64_t cur_asn, List *ka_to_transmit, uint32_t min_asn_per_ka);
void rplRxKA(Node_t *txNode, Node_t *rxNode, bool succes, uint8_t prr);

void rplScheduleDIO(List *dio_to_transmit, Node_t *node, uint64_t asn);
void rplScheduleKA(List *ka_to_transmit, Node_t *node, uint64_t asn);

uint64_t rplGetNextDIOASN(Node_t *node, uint32_t min_asn_per_dio);
uint64_t rplGetNextKAASN(Node_t *node, uint32_t min_asn_per_ka);

bool rplProcessTXDIO(uint8_t rpl_alg, Node_t *txNode, List *nodesList, uint8_t prrMatrix[][MAX_NODES][NUM_CHANNELS], uint8_t freq, List *dio_to_transmit, List *ka_to_transmit, uint32_t min_asn_per_dio, uint32_t min_asn_per_ka);
bool rplProcessTXKA(uint8_t rpl_alg, Node_t *txNode, List *nodesList, uint8_t prrMatrix[][MAX_NODES][NUM_CHANNELS], uint8_t freq);

Node_t *rplPreferedParent(Node_t *node, List *nodesList);

#endif // _RPL_H_

