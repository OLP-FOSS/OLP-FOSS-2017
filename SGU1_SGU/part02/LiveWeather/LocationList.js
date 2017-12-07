import React, { Component } from 'react';
import { View, Text, Platform, FlatList, TouchableOpacity, Image } from 'react-native';
import axios from 'axios';

const paddingTop = Platform.select({
  ios: 22,
  android: 0
})

locationIcon = require('./location.png')

export default class LocationList extends Component {

  constructor() {
    super();
    this.state = {
      data: []
    }
    this.renderItem = this.renderItem.bind(this);
  }

  componentDidMount() {
    axios.get('http://localhost:1880/devices').then(res => {
      this.setState({
        data: res.data.map((item, i) => {
          return { ...item, key: i }
        })
      })
    }).catch(err => {
    });
  }

  renderItem(data) {
    const { item } = data;
    const { navigate } = this.props.navigation
    return (
      <TouchableOpacity activeOpacity={0.7} onPress={() => {
        navigate('Detail', {
          device_id: item.id,
          location_detail: item
        })
      }}>
        <View style={{
          paddingVertical: 5,
          flexDirection: 'row',
          borderBottomWidth: 1,
          borderBottomColor: '#ccc'
        }}>
          <Image source={locationIcon} style={{ width: 32, height: 32, margin: 2 }} />
          <View style={{ justifyContent: 'space-between' }}>
            <Text style={{ color: '#000', fontWeight: 'bold' }}>{item.name}</Text>
            <Text style={{}}>
              {item.location}
            </Text>
          </View>
        </View>
      </TouchableOpacity>
    )
  }

  render() {
    return (
      <FlatList
        style={{
          flex: 1,
          paddingTop
        }}
        data={this.state.data}
        renderItem={this.renderItem}
      />
    )
  }

}