

#include <netinet/in.h>

#include <emproto.h>
//#include "epphyrep.h"   //Why this one is not exist in epmacrep.c

int epf_phyrep_rep(
	char *          buf,
	unsigned int    size,
	ep_phyrep_det * report)
{
	ep_phyrep_rep * rep = (ep_phyrep_rep *)buf;  // Cast message structure on buf

	if(size < sizeof(ep_phyrep_rep)) { // Checking if message will be longer than given buffer
		ep_dbg_log("F - MREP Rep: Not enough space!\n");
		return -1;
	}

	rep->tx_gain   = report->tx_gain;


	ep_dbg_dump(EP_DBG_2"F - MREP Rep: ", buf, sizeof(ep_phyrep_rep));

	return sizeof(ep_phyrep_rep);  // Return the header size
}

int epp_phyrep_rep(
	char *          buf,
	unsigned int    size,
	ep_phyrep_det * report)
{
	ep_phyrep_rep * rep = (ep_phyrep_rep *)buf;

	if(size < sizeof(ep_phyrep_rep)) {
		ep_dbg_log("P - MREP Rep: Not enough space!\n");
		return -1;
	}

	if(report) {
		report->tx_gain   = rep->tx_gain;

	}

	ep_dbg_dump(EP_DBG_2"P - MREP Rep: ", buf, sizeof(ep_phyrep_rep));

	return EP_SUCCESS;
}

int epf_phyrep_req(char * buf, unsigned int size, uint16_t interval, ep_phyrep_det * det)
{
	ep_phyrep_req * req = (ep_phyrep_req *)buf;

	if(size < sizeof(ep_phyrep_req)) {
		ep_dbg_log(EP_DBG_2"F - MREP Req: Not enough space!\n");
		return -1;
	}

	req->interval = htons(interval);
	req->tx_gain  = htonl(det->tx_gain);

	ep_dbg_dump(EP_DBG_2"F - MREP Req: ", buf, sizeof(ep_phyrep_req));

	return sizeof(ep_phyrep_req);
}

int epp_phyrep_req(char * buf, unsigned int size, uint16_t * interval, ep_phyrep_det * det)
{
	ep_phyrep_req * req = (ep_phyrep_req *)buf;

	if(size < sizeof(ep_phyrep_req)) {
		ep_dbg_log(EP_DBG_2"P - MREP Req: Not enough space!\n");
		return -1;
	}

	if(interval) {
		*interval    = ntohs(req->interval);
		det->tx_gain = ntohl(req->tx_gain);
	}

	ep_dbg_dump(EP_DBG_2"P - MREP Req: ", buf, sizeof(ep_phyrep_req));

	return EP_SUCCESS;
}

/******************************************************************************
 * Public API                                                                 *
 ******************************************************************************/

int epf_trigger_phyrep_rep_fail(
	char *       buf,
	unsigned int size,
	enb_id_t     enb_id,
	cell_id_t    cell_id,
	mod_id_t     mod_id)
{
	int ms = 0;
	int ret= 0;

	if(!buf) {
		ep_dbg_log(EP_DBG_0"F - Single phyREP Fail: Invalid buffer!\n");
		return -1;
	}

	ms = epf_head(
		buf,
		size,
		EP_TYPE_TRIGGER_MSG,
		enb_id,
		cell_id,
		mod_id,
		EP_HDR_FLAG_DIR_REP);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_trigger(
		buf + ret,
		size - ret,
		EP_ACT_PHY_REPORT,
		EP_OPERATION_FAIL);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_phyrep_rep(buf + ret, size - ret, 0);

	if(ms < 0) {
		return ms;
	}

	ret += ms;

	epf_msg_length(buf, size, ret);

	return ret;
}

int epf_trigger_phyrep_rep_ns(
	char *       buf,
	unsigned int size,
	enb_id_t     enb_id,
	cell_id_t    cell_id,
	mod_id_t     mod_id)
{
	int ms = 0;
	int ret= 0;

	if(!buf) {
		ep_dbg_log(EP_DBG_0"F - Single phyREP NS: Invalid buffer!\n");
		return -1;
	}

	ms = epf_head(
		buf,
		size,
		EP_TYPE_TRIGGER_MSG,
		enb_id,
		cell_id,
		mod_id,
		EP_HDR_FLAG_DIR_REP);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_trigger(
		buf + ret,
		size - ret,
		EP_ACT_PHY_REPORT,
		EP_OPERATION_NOT_SUPPORTED);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_phyrep_rep(buf + ret, size - ret, 0);

	if(ms < 0) {
		return ms;
	}

	ret += ms;

	epf_msg_length(buf, size, ret);

	return ret;
}

int epf_trigger_phyrep_rep(
	char *          buf,
	unsigned int    size,
	enb_id_t        enb_id,
	cell_id_t       cell_id,
	mod_id_t        mod_id,
	ep_phyrep_det * det)
{
	int ms = 0;
	int ret= 0;

	if(!buf || !det) {
		ep_dbg_log(EP_DBG_0"F - Single phyREP Rep: Invalid buffer!\n");
		return -1;
	}

	ms = epf_head(   // Format the header into buf
		buf,
		size,
		EP_TYPE_TRIGGER_MSG,
		enb_id,
		cell_id,
		mod_id,
		EP_HDR_FLAG_DIR_REP);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_trigger( // Format the trigger header into buf, after main header
		buf + ret,
		size - ret,
		EP_ACT_PHY_REPORT,
		EP_OPERATION_SUCCESS);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_phyrep_rep(buf + ret, size - ret, det);  // Real encoding of message

	if(ms < 0) {
		return ms;
	}

	ret += ms;

	epf_msg_length(buf, size, ret); // Fill the main header with the right message size

	return ret;
}

int epp_trigger_phyrep_rep(
	char *          buf,
	unsigned int    size,
	ep_phyrep_det * det)
{
	if(!buf) {
		ep_dbg_log(EP_DBG_0"P - Single phyREP Rep: Invalid buffer!\n");
		return -1;
	}

	return epp_phyrep_rep(
		buf + sizeof(ep_hdr) + sizeof(ep_t_hdr),
		size,
		det);
}

int epf_trigger_phyrep_req(
	char *       buf,
	unsigned int size,
	enb_id_t     enb_id,
	cell_id_t    cell_id,
	mod_id_t     mod_id,
	uint16_t     interval,
	ep_phyrep_det * det)
{
	int ms = 0;
	int ret= 0;

	if(!buf) {
		ep_dbg_log(EP_DBG_0"F - Single phyREP Req: Invalid buffer!\n");
		return -1;
	}

	ms = epf_head(
		buf,
		size,
		EP_TYPE_TRIGGER_MSG,
		enb_id,
		cell_id,
		mod_id,
		EP_HDR_FLAG_DIR_REQ);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_trigger(
		buf + ret,
		size - ret,
		EP_ACT_PHY_REPORT,
		EP_OPERATION_UNSPECIFIED);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	ms   = epf_phyrep_req(buf + ret, size - ret, interval, det);

	if(ms < 0) {
		return ms;
	}

	ret += ms;
	epf_msg_length(buf, size, ret);

	return ret;
}

int epp_trigger_phyrep_req(
	char *          buf,
	unsigned int    size,
	uint16_t *      interval,
	ep_phyrep_det * det)
{
	if(!buf) {
		ep_dbg_log(EP_DBG_0"P - Single phyREP Req: Invalid buffer!\n");
		return -1;
	}

	return epp_phyrep_req(
		buf + sizeof(ep_hdr) + sizeof(ep_t_hdr),
		size,
		interval,
		det);
}




