//
//  ViewController.swift
//  hackhuawei
//
//  Created by 曾兆阳 on 2017/1/2.
//  Copyright © 2017年 曾兆阳. All rights reserved.
//

import UIKit

class LoginViewController: UIViewController, UITextFieldDelegate {
    
    var backgroundView: UIImageView?
    var whiteView: UIImageView?
    var logoView: UIImageView?
    var closeView: UIImageView?

    var usernameInput: UITextField?
    var passwordInput: UITextField?
    
    var loginButton: UIButton?
    var registerButton: UIButton?
    
    let kScreenWidth = UIScreen.main.bounds.width
    let kScreenHeight = UIScreen.main.bounds.height
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
//        self.navigationController?.isNavigationBarHidden = true
        
        self.backgroundView = UIImageView(frame: self.view.bounds)
        self.backgroundView?.image = #imageLiteral(resourceName: "background")
        
        self.whiteView = UIImageView(image: #imageLiteral(resourceName: "whiteBox_halfCirle"))
        
        self.logoView = UIImageView(image: #imageLiteral(resourceName: "logo"))
        self.logoView?.frame.origin.x = (kScreenWidth - self.logoView!.frame.width) / 2
        self.logoView?.frame.origin.y = 60
        
        self.closeView = UIImageView(image: #imageLiteral(resourceName: "cross"))
        self.closeView?.frame.origin.x = (kScreenWidth - self.closeView!.frame.width) / 2
        self.closeView?.frame.origin.y = 617

        self.view.addSubview(self.backgroundView!)
        self.view.addSubview(self.whiteView!)
        self.view.addSubview(self.logoView!)
        self.view.addSubview(self.closeView!)
        
        self.usernameInput = UITextField(frame: CGRect(x: (kScreenWidth - 350) / 2, y: 305, width: 350, height: 48))
        self.usernameInput?.backgroundColor = UIColor.orange
        self.usernameInput?.borderStyle = .line
        self.usernameInput?.contentVerticalAlignment = .center
        self.usernameInput?.contentHorizontalAlignment = .center
        self.usernameInput?.textAlignment = .center
        self.usernameInput?.placeholder = "用户名"

        self.passwordInput = UITextField(frame: CGRect(x: (kScreenWidth - 350) / 2, y: 371, width: 350, height: 48))
        self.passwordInput?.backgroundColor = UIColor.orange
        self.passwordInput?.borderStyle = .line
        self.passwordInput?.contentVerticalAlignment = .center
        self.passwordInput?.contentHorizontalAlignment = .center
        self.passwordInput?.textAlignment = .center
        self.passwordInput?.isSecureTextEntry = true
        self.passwordInput?.delegate = self
        self.passwordInput?.placeholder = "密码"
        
        self.loginButton = UIButton(frame: CGRect(x: (kScreenWidth - 94) / 2, y: 460, width: 94, height: 18))
//        self.loginButton?.backgroundColor = UIColor.blue
        self.loginButton?.setTitle("登录", for: .normal)
        self.loginButton?.setBackgroundImage(#imageLiteral(resourceName: "ellipse"), for: .selected)
        self.loginButton?.setBackgroundImage(#imageLiteral(resourceName: "ellipse"), for: .highlighted)
        self.loginButton?.setTitleColor(UIColor.black, for: .normal)
        self.loginButton?.titleLabel?.font = UIFont.systemFont(ofSize: 30)
        self.loginButton?.titleLabel?.sizeToFit()
        self.loginButton?.addTarget(self, action: #selector(LoginViewController.loginButtonClick), for: .touchUpInside)
        
//        self.loginButton?.frame.origin = CGPoint(x: (kScreenWidth - self.loginButton!.frame.size.width) / 2, y: 400)
//        self.registerButton = UIButton(frame: CGRect(x: (kScreenWidth - 200) / 2 + 120, y: 390, width: 80, height: 50))
//        self.registerButton?.backgroundColor = UIColor.green
//        self.registerButton?.setTitle("Register", for: .normal)



//        print("haha")
        
        self.view.addSubview(self.usernameInput!)
        self.view.addSubview(self.passwordInput!)
        self.view.addSubview(self.loginButton!)
//        self.view.addSubview(self.registerButton!)
        
        // Do any additional setup after loading the view, typically from a nib.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func loginButtonClick() {
        
        let network = HWNetwork(path: "/login")
        network.request.httpBody = "username=\(self.usernameInput!.text!)&password=\(self.passwordInput!.text!)".data(using: String.Encoding.utf8)
        let dataTask: URLSessionTask = network.session.dataTask(with: network.request as URLRequest) { (data, resp, err) in
            let res = NSString(data: data!, encoding: String.Encoding.utf8.rawValue)
            if res == "1" {
                let navController = HWNavigationController()
                let homeViewController = HomeViewController()
                homeViewController.username = self.usernameInput!.text
                print("aaa")
                homeViewController.title = self.usernameInput!.text
                print("bbb")
                navController.pushViewController(homeViewController, animated: false)
                navController.modalTransitionStyle = .crossDissolve
                self.modalTransitionStyle = .crossDissolve
                DispatchQueue.main.async {
                    self.present(navController, animated: true) {
                        //
                    }
                }
                
            }
            else {
                DispatchQueue.main.async {
                    let HUD = MBProgressHUD(view: self.view)
                    self.view.addSubview(HUD!)
                    HUD?.labelText = "用户名或密码错误"
                    HUD?.mode = .text
                    HUD?.dimBackground = true
                    HUD?.show(animated: true, whileExecuting: {
                        sleep(1)
                        }, completionBlock: {
                            HUD?.removeFromSuperview()
                    })
                }
                
            }
           
        }
        dataTask.resume()
    }

    func textFieldShouldReturn(_ textField: UITextField) -> Bool {
        textField.resignFirstResponder()
        return true
    }

}

