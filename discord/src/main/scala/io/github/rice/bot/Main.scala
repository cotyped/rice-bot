package io.github.rice.bot

import java.io.{BufferedReader, InputStreamReader}

import com.fazecast.jSerialComm.SerialPort

object Main extends App {
  val port = SerialPort.getCommPorts()(0)
  port.openPort()
  port.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0)
  val reader = new BufferedReader(new InputStreamReader(port.getInputStream))
  reader.lines().forEach(x => println(x))
}
