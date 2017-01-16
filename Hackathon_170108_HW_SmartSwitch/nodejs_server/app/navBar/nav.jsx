import React,{Component} from 'react'
import ReactCSSTransitionGroup from 'react-addons-css-transition-group'
import NavItem from './navItems.jsx'
import { Menu, Icon } from 'antd';
import {Link} from 'react-router'
import {navLists} from '../StaticResource/navBarTitle'


export default class Nav extends Component {
    constructor(props, context) {
        super(props);
        this.state = {
            current: navLists.slice()[0].key
        };

        this.handleClick = this.handleClick.bind(this);
    }

    handleClick(e) {
        this.setState({
            current: e.key
        });
    }

    render() {
        return (
            <div>
                <div className="header">
                    <Menu className="menuBar" style={{paddingLeft:"300px"}} onClick={this.handleClick}
                          mode="horizontal"
                          selectedKeys={[this.state.current]}
                    >
                        {navLists.map((item) => {
                            return (<Menu.Item {...item}>
                                <Link to={item.path}><Icon type={item.icon}/>{item.title}</Link>

                            </Menu.Item>);
                        })}
                    </Menu>
                    <span className="logo">用专业的心 , 做专业的事 !</span>
                </div>
                <div className={"main-content"}>
                    <ReactCSSTransitionGroup
                        component="div"
                        transitionName="example"
                        transitionEnterTimeout={500}
                        transitionLeaveTimeout={500}
                    >
                        {React.cloneElement(this.props.children, {
                            key: this.props.location.pathname
                        })}
                    </ReactCSSTransitionGroup>
                </div>
                <footer className="footer">
                    <div className="centerBlock">
                        <div className="foot-nav f-l">
                            <ul>
                                <li className="light">
                                    <a href="#" target="_blank">专业服务</a>
                                </li>
                                <li className="light">
                                    <a href="#" target="_blank">产品展示</a>
                                </li>
                                <li className="light">
                                    <a href="#" target="_blank">最新资讯</a>
                                </li>
                                <li className="light noM">
                                    <a href="#" target="_blank">门店查询</a>
                                </li>
                                <li className="dark">
                                    <a href="#"target="_blank">关于我们</a>
                                </li>
                                <li className="dark">
                                    <a href="#" target="_blank">联系我们</a>
                                </li>
                                <li className="dark">
                                    <a href="#" target="_blank">加入我们</a>
                                </li>
                                <li className="dark">
                                    <a href="#" target="_blank">网站地图</a>
                                </li>
                                <li className="dark noM">
                                    <a href="#" target="_blank">会员专区</a>
                                </li>

                            </ul>
                        </div>
                        <div className="clearfix"></div>
                    </div>
                </footer>
            </div>
        );
    }
}

