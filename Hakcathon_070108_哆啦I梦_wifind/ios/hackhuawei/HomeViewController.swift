//
//  DeviceViewController.swift
//  hackhuawei
//
//  Created by 曾兆阳 on 2017/1/7.
//  Copyright © 2017年 曾兆阳. All rights reserved.
//

import UIKit

class HWDeviceCell: UITableViewCell {
    
    var dot: UIImageView?
    
    override init(style: UITableViewCellStyle, reuseIdentifier: String?) {
        super.init(style: UITableViewCellStyle.subtitle, reuseIdentifier: reuseIdentifier)
        self.dot = UIImageView(image: #imageLiteral(resourceName: "grayDot"))
        self.addSubview(self.dot!)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func layoutSubviews() {
        super.layoutSubviews()
        
        dot?.frame.origin = CGPoint(x: 25, y: (80 - dot!.frame.size.height) / 2)
        
        textLabel?.frame.origin = CGPoint(x: 60, y: 20)
        textLabel?.frame.size.width = 300
        textLabel?.font = UIFont.systemFont(ofSize: 20)
        
        detailTextLabel?.frame.origin = CGPoint(x: 60, y: 50)
        detailTextLabel?.frame.size.width = 300
        detailTextLabel?.font = UIFont.systemFont(ofSize: 14)
        detailTextLabel?.textColor = UIColor.gray
    }
}

private let reuseIdentifier = "Cell"

class HomeViewController: UIViewController, UITableViewDelegate, UITableViewDataSource {

    var backView: UIView?
    var nameLabel: UILabel?
    var macLabel: UILabel?
    var addButton: UIImageView?
    
    var deviceView: UITableView?
    
    var devices: NSMutableArray?
    
    let kScreenWidth = UIScreen.main.bounds.width
    let kScreenHeight = UIScreen.main.bounds.height
    
    var username: String?
    
    var HUD: MBProgressHUD?

    override func viewDidLoad() {
        super.viewDidLoad()

        self.view.backgroundColor = UIColor.white
        self.navigationController?.navigationBar.isTranslucent = false
        
        self.backView = UIView(frame: CGRect(x: 0, y: 0, width: kScreenWidth, height: 180))
        self.backView?.backgroundColor = UIColor(red: 237.0 / 255, green: 238.0 / 255, blue: 239.0 / 255, alpha: 1)
        
        self.nameLabel = UILabel()
        self.nameLabel?.text = ""
        self.nameLabel?.font = UIFont.systemFont(ofSize: 20)
        self.nameLabel?.textColor = UIColor.black
        self.nameLabel?.sizeToFit()
        self.nameLabel?.frame.origin = CGPoint(x: (kScreenWidth - self.nameLabel!.frame.size.width) / 2, y: 36)
        
        self.macLabel = UILabel()
        self.macLabel?.text = ""
        self.macLabel?.font = UIFont.systemFont(ofSize: 16)
        self.macLabel?.textColor = UIColor.gray
        self.macLabel?.sizeToFit()
        self.macLabel?.frame.origin = CGPoint(x: (kScreenWidth - self.macLabel!.frame.size.width) / 2, y: 70)
        
        self.HUD = MBProgressHUD(view: self.view)
        
        self.HUD?.dimBackground = true
        self.HUD?.labelText = "获取设备信息"

        
        self.addButton = UIImageView(image: #imageLiteral(resourceName: "orangeBox"))
        self.addButton?.frame.origin = CGPoint(x: (kScreenWidth - self.addButton!.frame.size.width) / 2, y: 100)
        let tapgesture: UITapGestureRecognizer = UITapGestureRecognizer(target: self, action: #selector(HomeViewController.addBtnClick))
        self.addButton?.isUserInteractionEnabled = true
        self.addButton?.addGestureRecognizer(tapgesture)
        
        
        self.devices = NSMutableArray()

        
        self.deviceView = UITableView(frame: CGRect(x: 0, y: 180, width: kScreenWidth, height: kScreenHeight - 180))
        self.deviceView?.dataSource = self
        self.deviceView?.delegate = self
        self.deviceView?.register(HWDeviceCell.self, forCellReuseIdentifier: reuseIdentifier)
        self.deviceView?.tableFooterView = UIView(frame: CGRect.zero)

        let logoutBtn = UIBarButtonItem(title: "注销", style: UIBarButtonItemStyle.plain, target: self, action: #selector(HomeViewController.logoutBtnClick))
        self.navigationItem.rightBarButtonItem = logoutBtn
        
        self.view.addSubview(self.backView!)
        self.view.addSubview(self.nameLabel!)
        self.view.addSubview(self.macLabel!)
        self.view.addSubview(self.addButton!)
        self.view.addSubview(self.deviceView!)
        self.view.addSubview(HUD!)

        
        self.HUD?.show(true)
        XPSummary.startSln { (ip, mac) in
            DispatchQueue.main.async {
                self.nameLabel?.text = "\(self.username!)的\(UIDevice.current.model)"
                self.nameLabel?.sizeToFit()
                self.nameLabel?.frame.origin = CGPoint(x: (self.kScreenWidth - self.nameLabel!.frame.size.width) / 2, y: 36)
                self.macLabel?.text = mac
                self.macLabel?.sizeToFit()
                self.macLabel?.frame.origin = CGPoint(x: (self.self.kScreenWidth - self.macLabel!.frame.size.width) / 2, y: 70)
                self.HUD?.removeFromSuperview()
                
            }
        }
    
        let network = HWNetwork(path: "/querydevice")
        network.request.httpBody = "username=\(self.username!)".data(using: String.Encoding.utf8)
        let dataTask: URLSessionTask = network.session.dataTask(with: network.request as URLRequest) { (data, resp, err) in
            var dict: NSDictionary? = nil
            do {
                dict = try! JSONSerialization.jsonObject(with: data!, options: .mutableContainers) as! NSDictionary
                self.devices = dict?["devices"] as? NSMutableArray
                for i in 0 ..< self.devices!.count {
                    if ((self.devices?[i] as! NSDictionary)["macaddr"] as! String == self.macLabel?.text) {
                        DispatchQueue.main.async {
                             self.addButton?.image = #imageLiteral(resourceName: "grayButton")
                        }
                    }
                }
                DispatchQueue.main.async {
                    self.deviceView?.reloadData()
                }
                
            }
            catch {
                
            }
        }
        
        dataTask.resume()
        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func numberOfSections(in tableView: UITableView) -> Int {
        return 1
    }
    
    func tableView(_ tableView: UITableView, heightForRowAt indexPath: IndexPath) -> CGFloat {
        return 80
    }
    
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return self.devices!.count
    }
    
    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = self.deviceView?.dequeueReusableCell(withIdentifier: reuseIdentifier, for: indexPath) as! HWDeviceCell
        let name = (self.devices![indexPath.row] as! NSMutableDictionary)["name"] as! String
        let macaddr = (self.devices![indexPath.row] as! NSMutableDictionary)["macaddr"] as! String
        
        cell.textLabel?.text = name
        cell.detailTextLabel?.text = macaddr
        return cell
    }
    
    func tableView(_ tableView: UITableView, canEditRowAt indexPath: IndexPath) -> Bool {
        return true
    }
    
    func tableView(_ tableView: UITableView, editingStyleForRowAt indexPath: IndexPath) -> UITableViewCellEditingStyle {
        return .delete
    }

    func tableView(_ tableView: UITableView, commit editingStyle: UITableViewCellEditingStyle, forRowAt indexPath: IndexPath) {
        
        let network = HWNetwork(path: "/deletedevice")
        network.request.httpBody = "username=\(self.username!)&macaddr=\((self.devices![indexPath.row] as! NSDictionary)["macaddr"]!)".data(using: String.Encoding.utf8)
        let dataTask: URLSessionTask = network.session.dataTask(with: network.request as URLRequest) { (data, resp, err) in
            DispatchQueue.main.async {
                self.devices?.removeObject(at: indexPath.row)
                self.deviceView?.deleteRows(at: [indexPath], with: .fade)
            }
        }
        dataTask.resume()
    }
    
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        let mapView = MapViewController()
        mapView.username = self.username
        mapView.title = (self.devices![indexPath.row] as! NSDictionary)["name"] as? String
        mapView.macaddr = (self.devices![indexPath.row] as! NSDictionary)["macaddr"] as? String
        self.navigationController?.pushViewController(mapView, animated: true)
    }
    
    func logoutBtnClick() {
        self.dismiss(animated: true, completion: nil)
    }

    func addBtnClick() {
        self.HUD?.labelText = "正在添加..."
        self.view.addSubview(self.HUD!)
        self.HUD?.show(true)
        let network = HWNetwork(path: "/adddevice")
        network.request.httpBody = "username=\(self.username!)&macaddr=\(self.macLabel!.text!)&name=\(self.nameLabel!.text!)&type=\(UIDevice.current.model)".data(using: String.Encoding.utf8)
        let dataTask: URLSessionTask = network.session.dataTask(with: network.request as URLRequest) { (data, resp, err) in
            let network2 = HWNetwork(path: "/querydevice")
            network2.request.httpBody = "username=\(self.username!)".data(using: String.Encoding.utf8)
            let dataTask2: URLSessionTask = network.session.dataTask(with: network2.request as URLRequest) { (data2, resp2, err2) in
                var dict: NSDictionary? = nil
                do {
                    dict = try! JSONSerialization.jsonObject(with: data2!, options: .mutableContainers) as! NSDictionary
                    self.devices = dict?["devices"] as? NSMutableArray
                    DispatchQueue.main.async {
                        self.deviceView?.reloadData()
                        self.addButton?.image = #imageLiteral(resourceName: "grayButton")
                        self.HUD?.removeFromSuperview()
                    }
                    
                }
                catch {
                    
                }
            }
            
            dataTask2.resume()
        }
        dataTask.resume()
    }
    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
