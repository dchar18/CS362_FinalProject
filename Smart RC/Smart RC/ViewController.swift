//
//  ViewController.swift
//  Smart RC
//
//  Created by Damian Charczuk on 2/24/20.
//  Copyright © 2020 Damian Charczuk. All rights reserved.
//

import UIKit
import CoreBluetooth

let hm10CBUUID = CBUUID(string: "0xFFE0")
let hm10CharacteristicCBUUID = CBUUID(string: "0xFFE1")
var hm10Characteristic : CBCharacteristic?

class ViewController: UIViewController {
    @IBOutlet weak var connectButton: UIButton!
    @IBOutlet weak var forwardButton: UIButton!
    @IBOutlet weak var backwardButton: UIButton!
    @IBOutlet weak var leftButton: UIButton!
    @IBOutlet weak var rightButton: UIButton!
    @IBOutlet weak var displayLabel: UILabel!
    
    var drive_value : String! = ""
    var steer_value : String! = ""
    
    var centralManager: CBCentralManager!
    var hm10Peripheral: CBPeripheral!
    
    var valueData : Data!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        centralManager = CBCentralManager(delegate: self, queue: nil)
        connectButton.setTitle("Connect", for: .normal)
        displayLabel.text = ""
    }
    

    // in value,
    // first digit: 1 = drive, 2 = steer
    // second digit: (drive) 1 = forward, 2 = backward
    //               (steer) 1 = left, 2 = right
    //               0 = stop
    @IBAction func onForwardButtonPressed(_ sender: UIButton) {
        print("Forward Pressed")
        forwardButton.backgroundColor = UIColor.lightGray
        drive_value = "11"
        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "1")
        
        backwardButton.isEnabled = false
        displayLabel.text = drive_value
    }
    
    @IBAction func onForwardButtonReleased(_ sender: UIButton) {
        print("Forward Released")
        forwardButton.backgroundColor = UIColor.darkGray
        drive_value = "10"
        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "0")
        backwardButton.isEnabled = true
        displayLabel.text = drive_value
    }
    
    @IBAction func onBackwardButtonPressed(_ sender: UIButton) {
        print("Back Pressed")
        backwardButton.backgroundColor = UIColor.lightGray
        drive_value = "12"
        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "2")
        forwardButton.isEnabled = false
        displayLabel.text = drive_value
    }
    
    @IBAction func onBackwardButtonReleased(_ sender: UIButton) {
        print("Back Released")
        backwardButton.backgroundColor = UIColor.darkGray
        drive_value = "10"
        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "0")
        forwardButton.isEnabled = true
        displayLabel.text = drive_value
    }
    
    @IBAction func onLeftButtonPressed(_ sender: UIButton) {
        print("Left Pressed")
        leftButton.backgroundColor = UIColor.lightGray
        steer_value = "21"
        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "1")
        rightButton.isEnabled = false
        displayLabel.text = steer_value
    }
    
    @IBAction func onLeftButtonReleased(_ sender: UIButton) {
        print("Left Released")
        leftButton.backgroundColor = UIColor.darkGray
        steer_value = "20"
        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "0")
        rightButton.isEnabled = true
        displayLabel.text = steer_value
    }
    
    @IBAction func onRightButtonPressed(_ sender: UIButton) {
        print("Right Pressed")
        rightButton.backgroundColor = UIColor.lightGray
        steer_value = "22"
        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "2")
        leftButton.isEnabled = false
        displayLabel.text = steer_value
    }
    
    @IBAction func onRightButtonReleased(_ sender: UIButton) {
        print("Right Released")
        rightButton.backgroundColor = UIColor.darkGray
        steer_value = "20"
        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "0")
        leftButton.isEnabled = true
        displayLabel.text = steer_value
    }
    
    
    func writeValue(data: String){
        let data = (data as NSString).data(using: String.Encoding.utf8.rawValue)
        if let peripheralDevice = hm10Peripheral{
            if let deviceCharacteristics = hm10Characteristic{
                peripheralDevice.writeValue((data)!, for: deviceCharacteristics, type: CBCharacteristicWriteType.withoutResponse)
            }
        }
    }
}

extension ViewController: CBCentralManagerDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .unknown:
            print("central.state is .unknown")
        case .resetting:
            print("central.state is .resetting")
        case .unsupported:
            print("central.state is .unsupported")
        case .unauthorized:
            print("central.state is .unauthorized")
        case .poweredOff:
            print("central.state is .poweredOff")
        case .poweredOn:
            print("central.state is .poweredOn")
            centralManager.scanForPeripherals(withServices: [hm10CBUUID])
        @unknown default:
            fatalError()
        }
    }
    
    func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print(peripheral)
        hm10Peripheral = peripheral
        hm10Peripheral.delegate = self
        centralManager.stopScan()
        central.connect(hm10Peripheral)
    }
    
    func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        print("Connected!")
        hm10Peripheral.discoverServices([hm10CBUUID])
    }
}

extension ViewController: CBPeripheralDelegate{
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else { return }
        
        for service in services {
            print(service)
            peripheral.discoverCharacteristics(nil, for: service)
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService,
                    error: Error?) {
        guard let characteristics = service.characteristics else { return }
        
        for characteristic in characteristics {
            print(characteristic)
            if characteristic.properties.contains(.read) {
                print("\(characteristic.uuid): properties contains .read")
                peripheral.readValue(for: characteristic)
            }
            if characteristic.properties.contains(.notify) {
                print("\(characteristic.uuid): properties contains .notify")
                peripheral.setNotifyValue(true, for: characteristic)
            }
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic,
                    error: Error?) {
        switch characteristic.uuid {
        case hm10CharacteristicCBUUID:
            print(characteristic.value ?? "no value")
            hm10Characteristic = characteristic
        default:
            print("Unhandled Characteristic UUID: \(characteristic.uuid)")
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        print("Value written")
    }
}

