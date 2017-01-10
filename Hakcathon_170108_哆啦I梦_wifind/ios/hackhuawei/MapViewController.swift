//
//  MapViewController.swift
//  hackhuawei
//
//  Created by 曾兆阳 on 2017/1/7.
//  Copyright © 2017年 曾兆阳. All rights reserved.
//

import UIKit
import MapKit

class PhoneAnnotationView: MKPinAnnotationView {
    
    var orangeBoxView: UIImageView?
    var bubbleView: UIImageView?
    var phoneView: UIImageView?
    
    
    override init(annotation: MKAnnotation?, reuseIdentifier: String?) {
        super.init(annotation: annotation, reuseIdentifier: reuseIdentifier)
        
        orangeBoxView = UIImageView(image: #imageLiteral(resourceName: "orangeSpot"))
        orangeBoxView?.frame.origin = CGPoint(x: (self.frame.size.width - (orangeBoxView?.frame.size.width)!) / 2, y: 0)
        
        bubbleView = UIImageView(image: #imageLiteral(resourceName: "bubble"))
        bubbleView?.frame.origin = CGPoint(x: (self.frame.size.width - (bubbleView?.frame.size.width)!) / 2, y: (orangeBoxView?.frame.origin.y)! - (bubbleView?.frame.size.height)! / 2 - 12)
        
        phoneView = UIImageView(image: #imageLiteral(resourceName: "iphone"))
        phoneView?.contentMode = .scaleAspectFill
        phoneView?.frame = CGRect(x: bubbleView!.frame.origin.x + bubbleView!.frame.size.width / 2 - 20, y: bubbleView!.frame.origin.y + bubbleView!.frame.size.height / 2 - 30, width: 40, height: 40)
        
        self.addSubview(orangeBoxView!)
        self.addSubview(bubbleView!)
        self.addSubview(phoneView!)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

private var reuseIdentifier = "phone"

class MapViewController: UIViewController, MKMapViewDelegate{

    var map: MKMapView?
    
    var location: CLLocationCoordinate2D?
    var macaddr: String?
    var username: String?
    
    override func viewDidLoad() {
        super.viewDidLoad()

        map = MKMapView(frame: self.view.bounds)
        
        map?.delegate = self
        self.view.addSubview(self.map!)
        
        let network = HWNetwork(path: "/querydetail")
//        print(self.username, self.macaddr)
        network.request.httpBody = "username=\(self.username!)&macaddr=\(self.macaddr!)".data(using: String.Encoding.utf8)
        let dataTask: URLSessionTask = network.session.dataTask(with: network.request as URLRequest) { (data, resp, err) in
            var dict: NSDictionary? = nil
            do {
               
                dict = try! JSONSerialization.jsonObject(with: data!, options: .mutableContainers) as! NSDictionary
                if (dict?["res"] as! String == "00000") {
                    self.location = CLLocationCoordinate2D(latitude: dict?["latitude"] as! Double, longitude: dict?["longitude"] as! Double)
                    
                    let place = MKCoordinateRegionMakeWithDistance(self.location!, 5000, 5000)
                    let annotation = MKPointAnnotation()
                    annotation.coordinate = self.location!
                    annotation.title = "哈哈哈"
                    annotation.subtitle = "呵呵呵"
                    DispatchQueue.main.async {
                        self.map?.setRegion(place, animated: false)
                        self.map?.addAnnotation(annotation)
                        self.map?.showsUserLocation = true
                        self.map?.selectedAnnotations = [annotation]
                    }
                }
                else {
                    DispatchQueue.main.async {
                        let HUD = MBProgressHUD(view: self.view)
                        self.view.addSubview(HUD!)
                        HUD?.labelText = "未发现设备位置"
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
            catch {
                
            }
        }
        dataTask.resume()
        
        
        // Do any additional setup after loading the view.
    }
    
    func mapView(_ mapView: MKMapView, viewFor annotation: MKAnnotation) -> MKAnnotationView? {
        let view = PhoneAnnotationView()
        return view as! MKAnnotationView
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
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
