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
    @IBOutlet weak var SensorSwitch: UISwitch!
    @IBOutlet weak var slider: UISlider!
    
    
    var drive_value : String! = ""
    var steer_value : String! = ""
    
    var centralManager: CBCentralManager!
    var hm10Peripheral: CBPeripheral!
    var connected : Bool!
    
    var valueData : Data!
    var direction : Int!
    var steering_enabled : Bool!
    var drive_enabled : Bool!
    
    var sensors_enabled : Bool!
    var slider_val : Float!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        connectButton.setTitle("Connect", for: .normal)
        displayLabel.text = ""
        direction = 0
        drive_enabled = false
        steering_enabled = false
        connected = false
        sensors_enabled = SensorSwitch.isOn
        slider_val = 0.5
        
    }
    @IBAction func onSensorSwitchChanged(_ sender: UISwitch) {
        if SensorSwitch.isOn {
            sensors_enabled = true
            writeValue(data: "A")
        }
        else{
            sensors_enabled = false
            writeValue(data: "B")
        }
        print(sensors_enabled!)
    }
    
    @IBAction func onSliderMoved(_ sender: Any) {
        if slider.value < 0.20{
            writeValue(data: "Z") // magenta
            print("Z")
        }
        else if slider.value < 0.40{
            writeValue(data: "Y") // red
            print("Y")
        }
        else if slider.value < 0.60{
            writeValue(data: "X") // green
            print("X")
        }
        else if slider.value < 0.80{
            writeValue(data: "W") // cyan
            print("W")
        }
        else{
            writeValue(data: "V") // blue
            print("V")
        }
        print(slider.value)
    }
    
    
    @IBAction func onConnectPressed(_ sender: UIButton) {
        if !connected{
            centralManager = CBCentralManager(delegate: self, queue: nil)
            connected = true
            connectButton.setTitle("Disconnect", for: .normal)
        }
        else{
            centralManager.cancelPeripheralConnection(hm10Peripheral)
            connected = false
            connectButton.setTitle("Connect", for: .normal)
        }
    }
    

    // in value,
    // first digit: 1 = drive, 2 = steer
    // second digit: (drive) 1 = forward, 2 = backward
    //               (steer) 1 = left, 2 = right
    //               0 = stop
    @IBAction func onForwardButtonPressed(_ sender: UIButton) {
        print("Forward Pressed")
        forwardButton.backgroundColor = UIColor.lightGray
//        drive_value = "11"
//        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "1")
    
        drive_enabled = true
        if !steering_enabled{
            direction = 1
            writeValue(data: String(direction))
        }
        else{
            if direction == 3{
                direction = 2
                writeValue(data: String(direction))
            }
            else{ //direction = 7
                direction = 8
                writeValue(data: String(direction))
            }
        }
        
        backwardButton.isEnabled = false
        displayLabel.text = String(direction)
    }
    
    @IBAction func onForwardButtonReleased(_ sender: UIButton) {
        print("Forward Released")
        forwardButton.backgroundColor = UIColor.darkGray
//        drive_value = "10"
//        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "0")
        
        drive_enabled = false
        if !steering_enabled{ // previously driving straight
            direction = 0
            writeValue(data: String(direction))
        }
        else{
            if direction == 8{
                direction = 7
                writeValue(data: String(direction))
            }
            else{ // direction = 2
                direction = 3
                writeValue(data: String(direction))
            }
        }
        
        backwardButton.isEnabled = true
//        displayLabel.text = drive_value
        displayLabel.text = String(direction)
    }
    
    @IBAction func onBackwardButtonPressed(_ sender: UIButton) {
        print("Back Pressed")
        backwardButton.backgroundColor = UIColor.lightGray
//        drive_value = "12"
//        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "2")
        
        drive_enabled = true
        if !steering_enabled{
            direction = 5
            writeValue(data: String(direction))
        }
        else{
            if direction > 5{ // direction = 7
                direction = 6
                writeValue(data: String(direction))
            }
            else{ //direction = 3
                direction = 4
                writeValue(data: String(direction))
            }
        }
        
        forwardButton.isEnabled = false
//        displayLabel.text = drive_value
        displayLabel.text = String(direction)
    }
    
    @IBAction func onBackwardButtonReleased(_ sender: UIButton) {
        print("Back Released")
        backwardButton.backgroundColor = UIColor.darkGray
//        drive_value = "10"
//        writeValue(data: String(drive_value))
//        writeValue(data: "1")
//        writeValue(data: "0")
        
        drive_enabled = false
        if !steering_enabled{ // previously driving straight
            direction = 0
            writeValue(data: String(direction))
        }
        else{
            if direction == 6{
                direction = 7
                writeValue(data: String(direction))
            }
            else{ // direction = 4
                direction = 3
                writeValue(data: String(direction))
            }
        }
        
        forwardButton.isEnabled = true
//        displayLabel.text = drive_value
        displayLabel.text = String(direction)
    }
    
    @IBAction func onLeftButtonPressed(_ sender: UIButton) {
        print("Left Pressed")
        leftButton.backgroundColor = UIColor.lightGray
//        steer_value = "21"
//        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "1")
        
        steering_enabled = true
        if !drive_enabled{
            direction = 7
            writeValue(data: String(direction))
        }
        else{
            if direction == 1{
                direction = 8
                writeValue(data: String(direction))
            }
            else{ // direction = 5
                direction = 6
                writeValue(data: String(direction))
            }
        }
        
        rightButton.isEnabled = false
//        displayLabel.text = steer_value
        displayLabel.text = String(direction)
    }
    
    @IBAction func onLeftButtonReleased(_ sender: UIButton) {
        print("Left Released")
        leftButton.backgroundColor = UIColor.darkGray
//        steer_value = "20"
//        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "0")
        
        steering_enabled = false
        if !drive_enabled{
            direction = 0
            writeValue(data: String(direction))
        }
        else{
            if direction == 8{
                direction = 1
                writeValue(data: String(direction))
            }
            else{ // direction = 6
                direction = 5
                writeValue(data: String(direction))
            }
        }
        
        
        rightButton.isEnabled = true
//        displayLabel.text = steer_value
        displayLabel.text = String(direction)
    }
    
    @IBAction func onRightButtonPressed(_ sender: UIButton) {
        print("Right Pressed")
        rightButton.backgroundColor = UIColor.lightGray
//        steer_value = "22"
//        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "2")
        
        steering_enabled = true
        if !drive_enabled{
            direction = 3
            writeValue(data: String(direction))
        }
        else{
            if direction == 1{
                direction = 2
                writeValue(data: String(direction))
            }
            else{ // direction = 5
                direction = 4
                writeValue(data: String(direction))
            }
        }
        
        leftButton.isEnabled = false
//        displayLabel.text = steer_value
        displayLabel.text = String(direction)
    }
    
    @IBAction func onRightButtonReleased(_ sender: UIButton) {
        print("Right Released")
        rightButton.backgroundColor = UIColor.darkGray
//        steer_value = "20"
//        writeValue(data: String(steer_value))
//        writeValue(data: "2")
//        writeValue(data: "0")
        
        steering_enabled = false
        if !drive_enabled{
            direction = 0
            writeValue(data: String(direction))
        }
        else{
            if direction == 2{
                direction = 1
                writeValue(data: String(direction))
            }
            else{ // direction = 4
                direction = 5
                writeValue(data: String(direction))
            }
        }
        
        leftButton.isEnabled = true
//        displayLabel.text = steer_value
        displayLabel.text = String(direction)
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

