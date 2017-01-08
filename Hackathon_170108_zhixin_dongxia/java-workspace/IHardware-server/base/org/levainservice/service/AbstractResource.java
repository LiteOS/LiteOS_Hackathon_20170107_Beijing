package org.levainservice.service;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

public class AbstractResource {
	private Gson gson;

	public AbstractResource() {
		gson = new GsonBuilder().setDateFormat("yyyy-MM-dd HH:mm:ss")

		.create();
	}

	public Gson getGson() {
		return gson;
	}

	public void setGson(Gson gson) {
		this.gson = gson;
	}

}