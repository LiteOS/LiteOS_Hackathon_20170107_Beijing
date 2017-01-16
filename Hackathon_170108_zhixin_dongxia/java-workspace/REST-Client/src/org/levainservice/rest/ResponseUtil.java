package org.levainservice.rest;

import com.google.gson.Gson;

public class ResponseUtil {
	public static final String METHOD_INVOKE_RESULT_SUCCESS = "success";

	public static final String METHOD_INVOKE_RESULT_FAIL = "fail";

	public static String getResponse(String status, String responseMessage) {
		Response response = new Response();
		response.setStatus(status);
		response.setResponseMessage(responseMessage);

		return new Gson().toJson(response);
	}

	public static String getSuccessResponse() {
		Response response = new Response();
		response.setStatus(METHOD_INVOKE_RESULT_SUCCESS);

		return new Gson().toJson(response);
	}

	public static String getFailResponse() {
		Response response = new Response();
		response.setStatus(METHOD_INVOKE_RESULT_FAIL);

		return new Gson().toJson(response);
	}
	
	public static Response createSuccessResponse(String msg){
		Response result = new Response();
		result.setStatus(ResponseUtil.METHOD_INVOKE_RESULT_SUCCESS);
		result.setResponseMessage(msg);

		return result;
	}
	
	public static Response createFailResponse(String msg){
		Response result = new Response();
		result.setStatus(ResponseUtil.METHOD_INVOKE_RESULT_FAIL);
		result.setResponseMessage(msg);

		return result;
	}
}
