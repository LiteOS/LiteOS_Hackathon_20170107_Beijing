//
//  File.swift
//  hackhuawei
//
//  Created by 曾兆阳 on 2017/1/7.
//  Copyright © 2017年 曾兆阳. All rights reserved.
//

import Foundation

class HWNetwork: NSObject {
    var session: URLSession
    var url: NSURL
    var request: NSMutableURLRequest
    
    init(path: String) {
        session = URLSession.shared
        url = NSURL(string: "http://10.1.18.7:5000" + path)!
        request = NSMutableURLRequest(url: url as URL)
        request.httpMethod = "POST"
    }

}
