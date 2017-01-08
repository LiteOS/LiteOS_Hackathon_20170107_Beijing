package org.levainservice.service.oms;

import java.util.List;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;

import org.apache.log4j.Logger;
import org.levainservice.rest.ResponseUtil;
import org.levainservice.service.AbstractResource;
import org.levainservice.service.oms.dao.IDeviceDAO;
import org.levainservice.service.oms.dao.impl.DeviceImpl;
import org.levainservice.service.oms.model.Device;

@Path("deviceService")
public class DeviceResource extends AbstractResource {
	Logger log = Logger.getLogger(DeviceResource.class);

	IDeviceDAO deviceDao = new DeviceImpl();

	/**
	 * 注册用户
	 * 
	 * @param json
	 *            device json字符串
	 * @return
	 */
	@POST
	@Path("/")
	@Produces(MediaType.APPLICATION_JSON)
	public String insert(String json) {
		log.trace("POST");
		log.info("sign in:" + json);
		try {
			Device device = this.getGson().fromJson(json, Device.class);

			boolean result = deviceDao.insert(device);
			if (result) {
				log.trace("device is Inserted ...  ok!");

				return ResponseUtil.getSuccessResponse();
			} else {
				log.trace("device is Inserted ...  error!");

				return ResponseUtil.getFailResponse();
			}
		} catch (Exception e) {
			return ResponseUtil.getResponse(
					ResponseUtil.METHOD_INVOKE_RESULT_FAIL,
					e.getLocalizedMessage());
		}
	}

	/**
	 * 获取设备列表
	 * 
	 * @param userId
	 *            用户编号
	 * @return
	 */
	@GET
	@Path("/{userId}")
	@Produces(MediaType.APPLICATION_JSON)
	public String signIn(@PathParam("userId") long userId) {
		log.trace("GET");
		log.info("get device list:" + userId);
		try {
			List<Device> devices = deviceDao.getDevicesByUser(userId);

			String json = this.getGson().toJson(devices);

			return ResponseUtil.getResponse(
					ResponseUtil.METHOD_INVOKE_RESULT_SUCCESS, json);
		} catch (Exception e) {
			return ResponseUtil.getResponse(
					ResponseUtil.METHOD_INVOKE_RESULT_FAIL,
					e.getLocalizedMessage());
		}
	}

}
