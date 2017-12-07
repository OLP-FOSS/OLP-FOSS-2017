import React, { Component } from 'react';
import { View, Text, Platform, ScrollView } from 'react-native';
import axios from 'axios';
import { locale } from 'core-js/library/web/timers';

const paddingTop = Platform.select({
  ios: 22,
  android: 0
})

export default class LocationDetail extends Component {

  constructor() {
    super();
    this.state = {
      data: {}
    }
  }

  componentDidMount() {
    const { device_id } = this.props.navigation.state.params;
    axios.get('http://localhost:1880/device/?id=' + device_id).then(res => {
      this.setState({
        data: res.data.data
      })
    }).catch(err => {
    });
  }

  render() {
    const { location_detail } = this.props.navigation.state.params;
    console.log(this.state.data)

    suggest = '';

    if (parseInt(this.state.data.temp) > 35) {
      suggest = 'Nhiệt độ tại vị trí này đang khá cao, bạn nên hạn chế ra ngoài vào thời điểm hiện tại!';
    } else if (parseInt(this.state.data.temp) > 20) {
      suggest = 'Nhiệt độ tại vị trí này khá lý tưởng!';
    } else {
      suggest = 'Nhiệt độ tại vị trí này đang khá thấp, bạn nên mặc thêm áo ấm!';
    }

    return (
      <ScrollView style={{
        flex: 1,
        paddingTop
      }}>
        <View style={{ alignItems: 'center', justifyContent: 'center', padding: 15, paddingTop: 10 }}>
          <Text style={{ fontWeight: 'bold', fontSize: 18 }}>{location_detail.name}</Text>
          <Text>{location_detail.location}</Text>
        </View>
        <View style={{ padding: 15 }}>
          <Text>Lời khuyên: {suggest}</Text>
        </View>
        <View style={{ padding: 15 }}>
          <View>
            <Text style={{ fontWeight: 'bold', marginBottom: 5 }}>Nhiệt độ hiện tại: <Text style={{ fontWeight: 'normal' }}>{this.state.data.temp} (C)</Text></Text>
            <Text style={{ fontWeight: 'bold' }}>Độ ẩm hiện tại: <Text style={{ fontWeight: 'normal' }}>{this.state.data.humidity} (%)</Text></Text>
          </View>
        </View>
      </ScrollView>
    )
  }

}