// pages/esp/esp.js
Page({

  /**
   * 页面的初始数据
   */
  data: {
   
   //蓝牙设备用到的变量
    deviceName:'esp_ceshi',//设备名字
    deviceId:'',//设备id
    connectedDeviceId:'',//连接id
    services:'',//服务参数
    notifyCharacteristicsId:'',
    notifyServicweId:'',
    balanceData:'',
    hexstr:'',
    
   //页面用到的
   ifConnet:0,//是否已经连接上
    background: { 
      sound:'#3aa6ff',
      stop: '#3aa6ff',
      center0: '#3aa6ff',
      center1: '#3aa6ff',
      center2: '#3aa6ff',
      center3: '#3aa6ff',
    },
    status:'小车停止',//当前状态
    datas: [0, 0, 0, 0, 0,0],//发送蓝牙数据，（0：0为前进，1为后退；1：前右pwm；2：前左pwm；3：后右pwm；4：后左pwm，最低为0，最高为6;5:喇叭）
    straight_speed:2,//转向变为直行时的速度
    turn_speed:4,//转向时四个轮子的初始速度
    last:0,//上一个动作，0停止，1加速，2减速，3偏左，4偏右
    max:9,//最大不可以超过的数
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {

  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {

  },

  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {

  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {

  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {

  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {

  },
//检测蓝牙是否可用
  getBluetoothAdapterState:function() {
    var that = this;
    
    
    wx.getBluetoothAdapterState({
      success: function (res) {
        wx.showLoading({
          title: '连接蓝牙中',
        })
        that.sleep(1500)
        that.startBluetoothDevicesDiscovery()
      },
      fail(res) {
     
     
        console.log(res)
      },
      complete(){
        
      }
    })
  },

  //开始搜索蓝牙设备
  startBluetoothDevicesDiscovery:function() {
    var that = this;
 
    setTimeout(() => {
      wx.startBluetoothDevicesDiscovery({
        success: function (res) {
          /* 获取蓝牙设备列表 */

          that.getBluetoothDevices()
        },
        fail(res) {
         
          that.sleep(1500)
          wx.hideLoading();
          wx.showToast({
            title: '连接失败',
            icon: 'none',
            duration: 1500
          })
        }
      })
    }, 1000)
  },

  //获取搜索到的蓝牙设备列表
  getBluetoothDevices:function() {
    var that = this;
    var check=0;
    setTimeout(() => {
      wx.getBluetoothDevices({
        services: [],
        allowDuplicatesKey: false,
        interval: 0,
        success: function (res) {
          if (res.devices.length > 0) {
           
              for (let i = 0; i < res.devices.length; i++) {
                if (that.data.deviceName === res.devices[i].name) {
                  check=1;
                  /* 根据指定的蓝牙设备名称匹配到deviceId */
                  that.setData({
                    deviceId : res.devices[i].deviceId
                  })
                 
                  console.log(i)
                  console.log(res.devices[i].deviceId)
                  setTimeout(() => {
                    that.connectTO(res.devices[i].deviceId);
                  }, 1000);
                  
                };
              };
           
           if(check==1);
           else{
             wx.hideLoading();
             wx.showToast({
               title: '连接失败',
               icon: 'none',
               duration: 1500
             })
           }
          } else {
            wx.hideLoading();
            wx.showToast({
              title: '连接失败',
              icon: 'none',
              duration: 1500
            })
          }
        },
        fail(res) {
          console.log(res, '获取蓝牙设备列表失败=====')
          wx.hideLoading();
          wx.showToast({
            title: '连接失败',
            icon: 'none',
            duration: 1500
          })
         
        }
      })
    }, 0)
  },

  //连接蓝牙
  connectTO(deviceId) {
    var that=this;
  
    wx.createBLEConnection({
      deviceId: deviceId,
      success: function (res) {
        that.setData({
          connectedDeviceId : deviceId
        })
        wx.hideLoading();
        wx.showToast({
          title: '连接成功',
          icon: 'success',
          duration: 1500
        })
        /* 4.获取连接设备的service服务 */
        that.getBLEDeviceServices(deviceId);
        wx.stopBluetoothDevicesDiscovery({
          success: function (res) {
            console.log(res, '停止搜索')
          },
          fail(res) {
            wx.hideLoading();
            wx.showToast({
              title: '连接失败1',
              icon: 'none',
              duration: 1500
            })
          }
        })
      

      },
      fail: function (res) {
        //console.log(111)
        console.log(res)
        wx.hideLoading();
        wx.showToast({
          title: '连接失败2',
          icon: 'none',
          duration: 1500
        })
      }
    })
  },

  //获取蓝牙设备服务
  getBLEDeviceServices(deviceId) {
    var that=this;
    
    setTimeout(() => {
      wx.getBLEDeviceServices({
        deviceId: deviceId,
        success: function (res) {
      
          console.log(res)
          that.setData({
            services :res.services,
            notifyServicweId: res.services[0].uuid
          })
         
          /* 获取连接设备的所有特征值 */
          that.getBLEDeviceCharacteristics()
        },
        fail: (res) => {
      
          console.log(res);
        }
      })
    }, 2000)
  },

  //获取蓝牙特征值
  getBLEDeviceCharacteristics() {
    var that=this;
    wx.showLoading({
      title: '获取特征',
    })
    setTimeout(() => {
      wx.getBLEDeviceCharacteristics({
        deviceId: that.data.connectedDeviceId,
        serviceId: that.data.notifyServicweId,
        success: function (res) {
          console.log(res)
          for (var i = 0; i < res.characteristics.length; i++) {
         if ((res.characteristics[i].properties.notify || res.characteristics[i].properties.indicate) &&
              (res.characteristics[i].properties.read && res.characteristics[i].properties.write)) {
           wx.hideLoading();
           wx.showToast({
             title: '获取成功',
             icon: 'success',
             duration: 1500
           })
              console.log(res.characteristics[i].uuid, '蓝牙特征值 ==========')
              /* 获取蓝牙特征值 */
              that.setData({
                notifyCharacteristicsId:res.characteristics[i].uuid,
                ifConnet:1,
              })
            //that.sendData("18")
              // 启用低功耗蓝牙设备特征值变化时的 notify 功能
              that.notifyBLECharacteristicValueChange()
              }
          }
        },
        fail: function (res) {
          wx.hideLoading();
          wx.showToast({
            title: '获取失败',
            icon: 'none',
            duration: 1500
          })
          console.log(res)
        }
      })
    }, 1000)
  },
// 启用低功耗蓝牙设备特征值变化时的 notify 功能
  notifyBLECharacteristicValueChange:function() { 
    var that = this;
    //console.log('6.启用低功耗蓝牙设备特征值变化时的 notify 功能')
    wx.notifyBLECharacteristicValueChange({
      state: true,
      deviceId: that.data.connectedDeviceId,
      serviceId: that.data.notifyServicweId,
      characteristicId: that.data.notifyCharacteristicsId,
      complete(res) {
        /*用来监听手机蓝牙设备的数据变化*/
        wx.onBLECharacteristicValueChange(function (res) {
          /**/
          var balanceData = that.data.balanceData;
          var hexstr = that.data.hexstr;
          balanceData += that.buf2string(res.value)
          hexstr += that.receiveData(res.value)
          that.setData({
            balanceData: balanceData,
            hexstr: hexstr
          })
        })
      },
      fail(res) {
        console.log(res, '启用低功耗蓝牙设备监听失败')
        
      }
    })
  },

  /*转换成需要的格式*/
  buf2string:function(buffer) {
    var arr = Array.prototype.map.call(new Uint8Array(buffer), x => x)
    return arr.map((char, i) => {
      return String.fromCharCode(char);
    }).join('');
  },
  receiveData:function(buf) {
    return this.hexCharCodeToStr(this.ab2hex(buf))
  },
  /*转成二进制*/
  ab2hex:function(buffer) {
    var hexArr = Array.prototype.map.call(
      new Uint8Array(buffer), function (bit) {
        return ('00' + bit.toString(16)).slice(-2)
      }
    )
    return hexArr.join('')
  },
  /*转成可展会的文字*/
  hexCharCodeToStr:function(hexCharCodeStr) {
    var trimedStr = hexCharCodeStr.trim();
    var rawStr = trimedStr.substr(0, 2).toLowerCase() === '0x' ? trimedStr.substr(2) : trimedStr;
    var len = rawStr.length;
    var curCharCode;
    var resultStr = [];
    for (var i = 0; i < len; i = i + 2) {
      curCharCode = parseInt(rawStr.substr(i, 2), 16);
      resultStr.push(String.fromCharCode(curCharCode));
    }
    return resultStr.join('');
  },

  //发送数据
  sendData:function(str) {
    let that = this;
    let dataBuffer = new ArrayBuffer(str.length)
    let dataView = new DataView(dataBuffer)
    for (var i = 0; i < str.length; i++) {
      
      dataView.setUint8(i, str.charCodeAt(i))
    }
   // console.log(dataView.getUint8(1))
   // let dataHex = that.ab2hex(dataBuffer);
    //this.writeDatas = that.hexCharCodeToStr(dataHex);
    wx.writeBLECharacteristicValue({
      deviceId: that.data.connectedDeviceId,
      serviceId: that.data.notifyServicweId,
      characteristicId: that.data.notifyCharacteristicsId,
      value: dataBuffer,
      success: function (res) {
        console.log('发送的数据：' + str)
        console.log('message发送成功')
      },
      fail: function (res) {
        console.log(res)
      },
      complete: function (res) {
      }
    })
  },

  // 断开设备连接
  closeConnect:function() {
    if (that.connectedDeviceId) {
      wx.closeBLEConnection({
        deviceId: that.connectedDeviceId,
        success: function (res) {
          that.closeBluetoothAdapter()
        },
        fail(res) {
        }
      })
    } else {
      that.closeBluetoothAdapter()
    }
  },
  // 关闭蓝牙模块
  closeBluetoothAdapter:function() {
    wx.closeBluetoothAdapter({
      success: function (res) {
      },
      fail: function (err) {
      }
    })
  },
  send_submit:function(e){
    var data = e.detail.value.send_txt;
    var that=this;
    that.sendData(data)

  },

  //点击连接小车
  conneting:function(){
    var that = this;
    //初始化蓝牙
    if (wx.openBluetoothAdapter) {
      wx.openBluetoothAdapter({
        success: function (res) {
          /* 获取本机的蓝牙状态 */
          wx.showToast({
            title: '初始化成功',
            icon: 'success',
            duration: 800
          })
          setTimeout(() => {
            that.getBluetoothAdapterState()
          }, 1000)
        },
        fail: function (err) {
          wx.showToast({
            title: '请开启蓝牙',
            icon: 'none',
            duration: 1500
          })
          // 初始化失败
          console.log(err);
        }
      })
    } else {

    }
  },
  //延时函数
  sleep:function(delay) {
    var start = (new Date()).getTime();
    while((new Date()).getTime() - start < delay) {
  continue;
}
}
,
//点击
  sound_start:function(e){
    var that=this;
    var background = that.data.background;
    var index = e.target.dataset.index;
    var datas = that.data.datas;
    var status = that.data.status;
    if(index==0)
    {
      //喇叭
      background.sound = '#ff6b6b';
      datas[5] = 1;
    }
    else if (index == 1) {
      //停止
     // background.sound = '#ff6b6b';
    }
    else if (index == 2) {
      //加速
      background.center0 = '#ff6b6b';
    }
    else if (index == 3) {
      //减速
      background.center1 = '#ff6b6b';
    }
    else if (index == 4) {
      //偏左
      background.center2 = '#ff6b6b';
        datas[2] = datas[4] =0;
        status = '小车左转';

    }
    else if (index == 5) {
      //偏右
      background.center3 = '#ff6b6b';

        datas[1] = datas[3] = 0;
        status = '小车右转';

    }
    var data = '';
    for (var i = 0; i < datas.length; i++) {
      data += datas[i];
    }
    console.log(data)
    that.sendData(data)
    that.setData({
      background: background,
      datas: datas,
      status: status

    })
  },
  //松开
  sound_end:function(e){
    var that = this;
    var background = that.data.background;
    var index = e.target.dataset.index;
    var datas = that.data.datas;
    var straight_speed = that.data.straight_speed;
    var turn_speed = that.data.turn_speed;
    var last = that.data.last;
    var max = that.data.max;
    var status = that.data.status;
    if (index == 0) {
      //喇叭
      background.sound = '#3aa6ff';
      datas[5] =0;
    }
    else if(index==1){
      //停止
      background.stop = '#ff6b6b';
      datas[0] = datas[1] = datas[2] = datas[3] = datas[4] =0;
      last=0;
      status='小车停止'
      console.log(datas);
    }
    else if (index == 2) {
      //加速
      background.center0 = '#3aa6ff';
      background.stop = '#3aa6ff'
      if (last==0){
        datas[1] = datas[2] = datas[3] = datas[4] = 1;
        datas[0] = 0;

      }
      else if (last == 1||last==2){
        if (datas[0]==0){
          if (datas[1] == max){
            //已经最快了，不做操作
            status = '直行满速';
          }
          else{
            datas[1] = datas[2] = datas[3] = datas[4] = datas[1]+1;
            status = '直行加速';
          }
        }
        else{
          if (datas[1] == 0){
            //当前速度为0，且是后退状态
            datas[0] =0;
            datas[1] = datas[2] = datas[3] = datas[4] =1;
            status = '小车直行';
          }
          else{
            datas[1] = datas[2] = datas[3] = datas[4] = datas[1]-1;
            status = '倒行减速';
          }
        }
      }
      else{
        //转向转直行
        datas[0] = 0;
        datas[1] = datas[2] = datas[3] = datas[4] = straight_speed;
        status='小车直行';
      }
      last =1;
      console.log(datas)
    }
    else if (index == 3) {
      //减速
      background.center1 = '#3aa6ff';
      background.stop = '#3aa6ff';
      if (last == 0) {
        datas[1] = datas[2] = datas[3] = datas[4] = 1;
        datas[0] = 1;

      }
      else if (last == 1 || last == 2) {
        if (datas[0] == 1) {
          if (datas[1] == max) {
            //已经最快了，不做操作
            status = '倒行满速';
          }
          else {
            datas[1] = datas[2] = datas[3] = datas[4] = datas[1] + 1;
            status = '倒行加速';
          }
        }
        else {
          if (datas[1] == 0) {
            //当前速度为0，且是后退状态
            datas[0] = 1;
            datas[1] = datas[2] = datas[3] = datas[4] = 1;
            status = '小车倒行';
          }
          else {
            datas[1] = datas[2] = datas[3] = datas[4] = datas[1] - 1;
            status = '直行减速';
          }
        }
      }
      else {
        //转向转直行
        datas[0] = 1;
        datas[1] = datas[2] = datas[3] = datas[4] = straight_speed;
        status = '小车倒行';
      }
      last = 2;
      console.log(datas)
    }
    else if (index == 4) {
      //偏左
      background.center2 = '#3aa6ff';
      background.stop = '#3aa6ff'
      /*if (last == 0 || last == 1 || last == 2 || last == 3){
        if (datas[2]==0){
          //速度最低了
          status = '最快左转';
        }
        else{
          datas[2] = datas[4] = datas[2] -1;
          status = '加快左转';
        }
      }
      else{
        datas[1] = datas[2] = datas[3] = datas[4] = datas[2];
        datas[2] = datas[4] = datas[2] - 1;
        status = '紧急左转';
      }

      last = 3;*/
      status = '左转结束';
      datas[1] = datas[2] = datas[3] = datas[4] = straight_speed;
      console.log(datas)
    }
    else if (index == 5) {
      //偏右
      background.center3 = '#3aa6ff';
      background.stop = '#3aa6ff'
    /*  if (last == 0 || last == 1 || last == 2 || last == 4) {
        if (datas[1] == 0) {
          //速度最低了
          status = '最快右转';
        }
        else {
          datas[1] = datas[3] = datas[1] - 1;
          status = '加快右转';
        }
      }
      else {
        datas[1] = datas[2] = datas[3] = datas[4] = datas[1];
        datas[1] = datas[3] = datas[1] - 1;
        status = '紧急右转';
      }
      last = 4;*/
      status = '右转结束';
      datas[1] = datas[2] = datas[3] = datas[4] = straight_speed;
      console.log(datas)
    } 
    var data='';
    for (var i = 0; i < datas.length;i++){
    data+=datas[i];
    }
    console.log(data)
    that.sendData(data)
    that.setData({
      background: background,
      datas: datas,
      last: last,
      status: status
    })
  }
})