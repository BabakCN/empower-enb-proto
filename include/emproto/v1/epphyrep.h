

/*    PHY REPORT
 *
 * This message provides details about PHY layer reporting
 */

#ifndef __EMAGE_CELL_PHY_REPORT_H
#define __EMAGE_CELL_PHY_REPORT_H

#include <stdint.h>

#include "eppri.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*
 * Cell phy report messages:
 */

typedef struct __ep_cell_phy_report_reply {   //request from controller


	uint32_t tx_gain;

}__attribute__((packed)) ep_phyrep_rep; // "Packed" means do not align in RAM

typedef struct __ep_cell_phy_report_request {
	uint16_t interval;
}__attribute__((packed)) ep_phyrep_req;

/******************************************************************************
 * Opaque structures                                                          *
 ******************************************************************************/

typedef struct ep_cell_phy_report_details {  //a layer at the middle

	uint32_t tx_gain;
	//uint32_t bw;

} ep_phyrep_det;

/******************************************************************************
 * Operation on single-event messages                                         *
 ******************************************************************************/

/******************************************************************************
 * Operation on schedule-event messages                                       *
 ******************************************************************************/

/******************************************************************************
 * Operation on trigger-event messages                                        *
 ******************************************************************************/

/* Format a phy report negative reply.
 * Returns the size of the message, or a negative error number.
 */
int epf_trigger_phyrep_rep_fail(
	char *       buf,
	unsigned int size,
	enb_id_t     enb_id,
	cell_id_t    cell_id,
	mod_id_t     mod_id);

/* Format a phy report not-supported reply.
 * Returns the size of the message, or a negative error number.
 */
int epf_trigger_phyrep_rep_ns(
	char *       buf,
	unsigned int size,
	enb_id_t     enb_id,
	cell_id_t    cell_id,
	mod_id_t     mod_id);

/* Format a phy report reply.
 * Returns the size of the message, or a negative error number.
 */
int epf_trigger_phyrep_rep(
	char *          buf,
	unsigned int    size,
	enb_id_t        enb_id,
	cell_id_t       cell_id,
	mod_id_t        mod_id,
	ep_phyrep_det * det);

/* Parse a phy report reply looking for the desired fields */
int epp_trigger_phyrep_rep(
	char *          buf,
	unsigned int    size,
	ep_phyrep_det * det);

/* Format a phy report request.
 * Returns the size of the message, or a negative error number.
 */
int epf_trigger_phyrep_req(
	char *          buf,
	unsigned int    size,
	enb_id_t        enb_id,
	cell_id_t       cell_id,
	mod_id_t        mod_id,
	/* Interval for statistic measurements, in 'ms' */
	uint16_t        interval);

/* Parse a phy report request looking for the desired fields */
int epp_trigger_phyrep_req(
	char *          buf,
	unsigned int    size,
	uint16_t *      interval);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EMAGE_CELL_phy_REPORT_H */
