package org.levainservice.rest.oms;

import org.levainservice.rest.RESTClient;
import org.levainservice.rest.Response;
import org.levainservice.service.oms.model.Device;
import org.levainservice.util.IdRandom;

import com.google.gson.Gson;

public class DeviceClient extends BaseClient {
	private static final String RESOURCE_PATH = "/deviceService";

	public Response insert(Device device) {
		long id = Long.parseLong(IdRandom.getId());
		device.setDeviceId(id);
		String json = gson.toJson(device);
		String servicePort = BaseClient.SERVICE_BASE_URL + RESOURCE_PATH + "/";

		Response response = client.request(servicePort,
				RESTClient.SUBMIT_METHOD_POST, json);
		return response;
	}

	public Response listDevices(long userId) {

		String servicePort = BaseClient.SERVICE_BASE_URL + RESOURCE_PATH + "/"
				+ userId;

		Response response = client.request(servicePort,
				RESTClient.SUBMIT_METHOD_GET, "");
		return response;
	}

	public static void main(String[] args) {
		DeviceClient client = new DeviceClient();

		Response r = client.listDevices(1);
		System.out.println(new Gson().toJson(r));
	}
}
