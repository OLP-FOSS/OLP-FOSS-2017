import React, { Component } from 'react';
import {
  Platform,
  StyleSheet,
  Text,
  View,
  FlatList
} from 'react-native';

import {
  StackNavigator,
} from 'react-navigation';


import LocationList from './LocationList';
import LocationDetail from './LocationDetail';

const AppStack = StackNavigator({
  List: { screen: LocationList },
  Detail: { screen: LocationDetail }
}, {
    headerMode: 'none',
    navigationOptions: {
      headerVisible: false,
    }
  })

export default class App extends Component {
  render() {
    return <AppStack />
  }
}
